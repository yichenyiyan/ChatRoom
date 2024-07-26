#include "command.h"

#include "chat_server.h"

static constexpr int thread_number = 2;
static constexpr int event_max_number = 1024;

chatServer::chatServer(const char* ip, unsigned short port)
    : m_bStop(false), m_listenfd(-1), m_accept_mutex(PTHREAD_MUTEX_INITIALIZER), 
    m_accept_cond(PTHREAD_COND_INITIALIZER), m_worker_mutex(PTHREAD_MUTEX_INITIALIZER), 
    m_worker_cond(PTHREAD_COND_INITIALIZER), m_group_send_mutex(PTHREAD_MUTEX_INITIALIZER),
     m_group_send_cond(PTHREAD_COND_INITIALIZER), function_mutex(PTHREAD_MUTEX_INITIALIZER), 
     function_cond(PTHREAD_COND_INITIALIZER), m_private_send_mutex(PTHREAD_MUTEX_INITIALIZER),
     m_private_send_cond(PTHREAD_COND_INITIALIZER), m_epollfd(-1)
{
    m_listenfd = listenSocketInit(ip, port);
    m_epollfd = epollInit(m_listenfd);
    
    // 启动接受连接的线程
    std::thread(&chatServer::Accept_thread_func, this).detach();

    // 启动工作线程
    for (int i = 0; i < thread_number + 1; ++i) 
        std::thread(&chatServer::Worker_thread_func, this).detach();
    
    
    std::thread(&chatServer::Group_Send_thread_func, this).detach();
    std::thread(&chatServer::Function_Send_thread_func, this).detach();
    std::thread(&chatServer::Alone_Send_thread_func, this).detach();

}
chatServer::~chatServer() {}

bool chatServer::Quit()
{
    m_bStop = true;

    /* 读端和写端都关闭 */
    shutdown(m_listenfd, SHUT_RDWR);
    close(m_listenfd);
    close(m_epollfd);

    return true;
}

bool chatServer::ClientClose(int clientfd)
{
    if (!removeSocketFromEpoll(m_epollfd, clientfd))
        return false;
    std::future<bool> res = this->asyncCancleUserOnline(clientfd);
    if (!res.get())    
        return false;
    
    std::cout << "close fd: "<< clientfd << std::endl;
    close(clientfd);

    return true;
}

void chatServer::main_loop() {
#ifdef TODEBUG
    std::cout << "main thread id = " << pthread_self() << std::endl;
#endif
    while (!m_bStop) {
        struct epoll_event ev[event_max_number] = {0};
        int n = epoll_wait(m_epollfd, ev, 1024, 10);
        if (n == 0)
            continue;
        else if (n < 0) {
#ifdef TODEBUG
            std::cout << "epoll_wait error" << std::endl;
#endif
            continue;
        }

        int m = std::min(n, event_max_number);
        for (int i = 0; i < m; i++) {
            /* 有新连接 */
            if (ev[i].data.fd == m_listenfd)
                pthread_cond_signal(&m_accept_cond);
            /* 有数据 */
            else {
                pthread_mutex_lock(&m_worker_mutex);
                new_event_fds.push(ev[i].data.fd);
                pthread_mutex_unlock(&m_worker_mutex);
                pthread_cond_signal(&m_worker_cond);
            }
        }
    }
}

void chatServer::Accept_thread_func() {
#ifdef TODEBUG
    std::cout << "accept thread id = " << pthread_self() << std::endl;
#endif

    while (!m_bStop) {
        pthread_mutex_lock(&m_accept_mutex);
        pthread_cond_wait(&m_accept_cond, &m_accept_mutex);

        struct sockaddr_in clientaddr;
        socklen_t addrlen;
        int newfd = accept(m_listenfd, (struct sockaddr *)&clientaddr, &addrlen);
        pthread_mutex_unlock(&m_accept_mutex);
        if (newfd == -1)
            continue;

        setSocketNonBlocking(newfd);
        addSocketToEpoll(m_epollfd, newfd);
    }
}

void chatServer::Worker_thread_func() {
#ifdef TODEBUG
    std::cout << "worker thread id = " << pthread_self() << std::endl;
#endif
    while (!m_bStop) {
        int clientfd;
        pthread_mutex_lock(&m_worker_mutex);
        /* 注意！要用while循环等待 */
        while (new_event_fds.empty())
            pthread_cond_wait(&m_worker_cond, &m_worker_mutex);

        /* 取出客户套接字 */
        clientfd = std::move(new_event_fds.front());
        new_event_fds.pop();
        pthread_mutex_unlock(&m_worker_mutex);

        
        std::string input;
        char buff[512];
        while (true) {
            memset(buff, 0, sizeof(buff));
            int nRecv = recv(clientfd, buff, 256, 0);

            /* recv error */
            if (nRecv == -1) {
                if (errno == EWOULDBLOCK)
                    break;
                else {
                    ClientClose(clientfd);
                    break;
                }
            } else if (nRecv == 0) { /* client disconnected */
                std::unordered_map<int ,std::string>::iterator it;
                if ((it = fd_onlineUser.find(clientfd)) != fd_onlineUser.end()) {
                    std::string lname = it->second;
                    std::cout << "leave username: " << lname << std::endl;
                    onlineUsers.erase(lname);
                    ClientClose(clientfd);
                } else {
                    removeSocketFromEpoll(m_epollfd, clientfd);
                    close(clientfd);
                    std::cout << "clientfd: "<< clientfd <<" disconnected"<<std::endl;
                }
                break;
            }

            std::string input;
            input.clear();
            input = buff;
            std::string tmp;

            switch (buff[0]) {
                case REGISTER: { /* 注册 ok*/
                    std::string username;
                    std::string password;
                    getUserNameAndPass(input, username, password);
                    if (doesUserExist(username)) {
                        send(clientfd, "1", sizeof("1"), 0);
                    } else {
                        if (insertNewUser(username, password) && createJoinedGroupsTable(username)) {
                            send(clientfd, "0", sizeof("0"), 0);
                        } else {
                            //send(clientfd, "server error", sizeof("server error"), 0);
                            send(clientfd, "2", sizeof("0"), 0);
                        }
                    }
                    break;
                }
                case LOGIN: { /* 验证性登录 ok*/
                    std::string username;
                    std::string password;
                    getUserNameAndPass(input, username, password);
                    if (!doesUserExist(username)) {
                        send(clientfd, "0", sizeof("0"), 0);
                    } else {
                        if (verifyPassword(username, password)) {
                            if (onlineUsers.find(username) != onlineUsers.end()) {
                                send(clientfd, "1", sizeof("1"), 0);
                            } else {
                                //std::future<void> res = this->asyncToRecordLogin(username, clientfd);
                                send(clientfd, "2", sizeof("2"), 0);
                            }
                        } else {
                            send(clientfd, "3", sizeof("3"), 0);
                        }
                    }
                    break;
                }
                case GMESSAGE: { /* 群消息 ok*/
                    std::cout << "\n原群消息：" << input << std::endl;
                    std::string user;
                    std::string group;
                    std::string msg;
                    std::pair<std::string, std::pair<std::string, std::string>> info;
                    processMessage(input, user, group, msg);
                    std::cout << "after parse: user " << user << " group " << group << " msg " << msg << std::endl;
                    info.first = group;
                    info.second.first = user;
                    info.second.second = msg;
                    group_msgs.push(info);
                    pthread_cond_signal(&m_group_send_cond);
                    break;
                }
                case ADDFRIEND: { /* 添加好友 ok*/
                    input.erase(input.begin(), input.begin() + 2);
                    std::string req_user;
                    std::string tar_user;
                    std::pair<int, std::string> add_friend_info;
                    parse_add_friend_cmd(input, req_user, tar_user);
                    

                    if (onlineUsers.find(req_user) == onlineUsers.end()) {
                        // send(clientfd, "please log in first", sizeof("please log in first"), 0);
                        send(clientfd, "4", 1, 0);
                        ClientClose(clientfd);
                    } else if (checkUserExists(tar_user)) {
                        if (checkFrinedExists(req_user, tar_user)) {
                            // send(clientfd, "He is already your friend", sizeof("He is already your friend"), 0);
                            send(clientfd, "2", 1, 0);
                        }
                        else if (onlineUsers.find(tar_user) == onlineUsers.end()) {
                            // send(clientfd, "The user not online", sizeof("The user not online"), 0);
                            send(clientfd, "3", 1, 0);
                        }
                        else {
                            add_friend_info.first = onlineUsers[tar_user];
                            add_friend_info.second = produce_add_friend_cmd(req_user, tar_user);
                            function_msgs.push(add_friend_info);
                            pthread_cond_signal(&function_cond);
                            send(clientfd, "1", 1, 0);
                            continue;
                        }

                    }
                    else
                    {
                        //send(clientfd, "The user you added does not exist", sizeof("The user you added does not exist"), 0);
                        send(clientfd, "0", 1, 0);
                    }
                    break;
                }
                case PMESSAGE: { /* 一对一消息 ok*/
                    std::pair<int, std::string> taruser_msg;
                    #ifndef TODEBUG
                    std::cout << "debug:"<<input<<std::endl;
                    #endif
                    std::string src_user;
                    std::string tar_user;
                    std::string msg;
                    splitInput(input,tmp, src_user, tar_user, msg);
                    if (checkFrinedExists(src_user, tar_user))
                    {
                        std::unordered_map<std::string, int>::const_iterator cit;
                        if ((cit = onlineUsers.find(tar_user)) != onlineUsers.end())
                        {
                            taruser_msg.first = cit->second;

                            taruser_msg.second = "5 " + src_user +" " + msg;
                            private_msgs.push(taruser_msg);
                            pthread_cond_signal(&m_private_send_cond);
                            continue;
                        } else {
                            //send(clientfd, "He/She is not online", sizeof("He/She is not online"), 0);
                            send(clientfd, "0", 1, 0);
                        }
                    } else {
                        //send(clientfd, "He/She is not your friend", sizeof("He/She is not your friend"), 0);
                        send(clientfd, "1", 1, 0);
                    }
                    break;
                }
                case FRIENDREPLY: { /* 好友申请回复 ok*/
                    //std::cout << "debug: friend reply raw data: " << input << std::endl;
                    input.erase(input.begin(), input.begin() + 2);
                    std::string src_user;
                    std::string tar_user;
                    std::string msg;
                    std::pair<int,std::string> addfriend_info;
                    std::pair<int,std::string> addfriend_info2;
                    get_src_tar_user_msg(input, src_user, tar_user, msg);
                    if ((msg == "accept" || msg == "reject" )&& checkUserExists(src_user) && checkUserExists(tar_user))
                    {
                        if (msg == "accept" && onlineUsers.find(src_user) != onlineUsers.end() &&
                            onlineUsers.find(tar_user) != onlineUsers.end()) {
                            if (add_friend(src_user, tar_user) && add_friend(tar_user, src_user)) {
                                addfriend_info.first = onlineUsers[tar_user];
                                addfriend_info.second = "5 system " + src_user + " "+  msg + " your friend request."; 
                                addfriend_info2.first = onlineUsers[src_user];
                                addfriend_info2.second = "5 system you " +  msg + " " + tar_user + "'s add friend request.";
                            }
                        } else if (msg == "reject" && onlineUsers.find(src_user) != onlineUsers.end() &&
                            onlineUsers.find(tar_user) != onlineUsers.end()) {
                            addfriend_info.first = onlineUsers[tar_user];
                            addfriend_info.second = "5 system " + src_user +" " + msg + " your friend request."; 
                            addfriend_info2.first = onlineUsers[src_user];
                            addfriend_info2.second = "5 system you " +  msg + " " + tar_user + "'s add friend request.";
                        }
                        function_msgs.push(addfriend_info);
                        pthread_cond_signal(&function_cond);
                        function_msgs.push(addfriend_info2);
                        pthread_cond_signal(&function_cond);
                        continue;
                    }
                    break;
                }
                case GETUSERFRIENDS: { /* 获取所有好友用户名 ok*/
                    input.erase(input.begin(), input.begin() + 2);
                    if (tableExists(input)) {
                        std::string res = fetchTableData(input);
                        std::string req = res;
                        std::cout << req << std::endl;
                        send(clientfd, req.c_str(), req.size(), 0);
                    } else {
                        send(clientfd, "0", 1, 0);
                    }
                    break;
                }
                case UPDATESOCKET: { /* 登记用户信息 ok*/
                    input.erase(input.begin(), input.begin() + 2);
                    std::cout << input << " 登记用户信息"<< std::endl;
                    std::future<bool> res = this->asyncToRecordLogin(input, clientfd);
                    if (res.get())
                        send(clientfd, "OK", 2, 0);
                    else 
                        send(clientfd, "NO", 2, 0);
                    break;
                }
                case CREATEGROUP: { /* 创建群聊 ok*/
                    input.erase(input.begin(), input.begin() + 2);
                    std::string GroupName;
                    std::string createUserName;
                    groupGetCreateUserNameAndGroupName(input, createUserName, GroupName);
                    if (groupExists(GroupName)) {                        
                        send(clientfd, "1", 1, 0); /* 群聊已存在 */
                        continue;
                    } else {
                        if (createGroup(GroupName, createUserName)
                            && addGroupToUserJoinedGroups(createUserName, GroupName)) {
                            send(clientfd, "0", 1, 0); /* 成功 */
                        } else {
                            send(clientfd, "2", 1, 0); /* 失败 */
                        }
                        continue;
                    }
                    break;
                }
                case TOJOINGROUNP: { /* 加入群聊申请 ok*/
                    input.erase(input.begin(), input.begin() + 2);
                    std::string GroupName;
                    std::string reqUserName;
                    groupGetCreateUserNameAndGroupName(input, reqUserName, GroupName);
                    if (groupExists(GroupName)) {
                        if (userInGroup(reqUserName, GroupName)) {
                            send(clientfd, "1", 1, 0); /* 该用户已加入该群聊 */
                        } else {
                            std::pair<int, std::string> master_info;
                            std::unordered_map<std::string, int>::iterator find_master;
                            if ((find_master = onlineUsers.find(getGroupMasterUser(GroupName))) != onlineUsers.end()) { /* 群主在线 */
                                master_info.first = find_master->second;
                                master_info.second = "d " + reqUserName + " " + GroupName;
                                send(clientfd, "3", 1, 0); /* 请求发送成功 */
                                function_msgs.push(master_info);
                                pthread_cond_signal(&function_cond);
                            } else {
                                send(clientfd, "2", 1, 0); /* 群主不在线 */
                            }
                            // if (addUserToGroup(reqUserName, GroupName, false))
                            //     send(clientfd, "2", 1, 0); /* 加入成功 */

                            // else 
                            //     send(clientfd, "3", 1, 0); /* 加入失败 */
                        }
                        continue;
                    } else {
                        send(clientfd, "0", 1, 0); /* 群聊不存在 */
                        continue;
                    }
                    break;
                }
                case GETUSERJOINEDGROUP: { /* 获取某个用户已加入的所有群聊 ok*/
                    input.erase(input.begin(), input.begin() + 2);
                    // std::string username = input;
                    std::cout << "debug get input: " << input << std::endl;
                    std::pair<int, std::string> getUserAllGroups;
                    getUserAllGroups.first = clientfd;
                    if (!doesUserExist(input)) {
                        getUserAllGroups.second = "user not exist!";
                    } else {
                        if (doesJoinedGroupsTableExist(input))
                            getUserAllGroups.second = getJoinedGroups(input);
                        else 
                            getUserAllGroups.second = "";
                    }
                    function_msgs.push(getUserAllGroups);
                    pthread_cond_signal(&function_cond);
                    break;
                }
                case GETGROUPALLUSERS: { /* 获取某个群聊的所有用户名 ok*/
                    input.erase(input.begin(), input.begin() + 2);
                    std::pair<int, std::string> getGroupAllUsers;
                    getGroupAllUsers.first = clientfd;
                    if (groupExists(input)) {
                        getGroupAllUsers.second = getUsersInGroup(input);
                    } else {
                        getGroupAllUsers.second = "0"; /* 群聊不存在 */
                    }

                    function_msgs.push(getGroupAllUsers);
                    pthread_cond_signal(&function_cond);
                    break;
                }
                case ADDGROUPREPLY: { /* 加入群聊回复 TODO*/
                    input.erase(input.begin(), input.begin() + 2);
                    std::string master;
                    std::string tar_user;
                    std::string groupName;
                    std::string master_reply;
                    std::pair<int, std::string> info;
                    get_src_tar_group_msg(input, master, tar_user, groupName, master_reply);
                    if (master_reply == "accept" || master_reply == "reject") {
                        info.first = onlineUsers[tar_user];
                        info.second = "5 system " + master +" " + master_reply + " you join " + groupName;
                        std::string reply = "5 system you " + master_reply + " " + tar_user + " join Group " + groupName;
                        send(clientfd, reply.c_str(), reply.size(), 0);
                        if (master_reply == "accept" && addUserToGroup(tar_user, groupName, false) && addGroupToUserJoinedGroups(tar_user, groupName)) {
                        #ifndef TODEBUG
                        	std::cout << tar_user <<" add " << groupName << " success and" << tar_user << " joined tables success!" << std::endl;
                        #endif
                        }
                    }
                    function_msgs.push(info);
                    pthread_cond_signal(&function_cond);
                    break;
                }
                default: {
                    /* TODO */
                    // 处理未识别的命令
                    break;
                }
            }
        }
    }


}


void chatServer::Group_Send_thread_func() {
#ifdef TODEBUG
    std::cout << "group send thread id = " << pthread_self() << std::endl;
#endif
    while (!m_bStop) {

        pthread_mutex_lock(&m_group_send_mutex);
        /* 注意！要用while循环等待 */
        while (group_msgs.empty())
            pthread_cond_wait(&m_group_send_cond, &m_group_send_mutex);

        auto _group = group_msgs.front().first;
        auto _sender = group_msgs.front().second.first;
        auto _msg = group_msgs.front().second.second;
        group_msgs.pop();
        pthread_mutex_unlock(&m_group_send_mutex);
        auto it = groups.find(_group);
        if (it->second.empty()) {
            std::cout << "debug1: 群聊 " << _group << " 当前在线人数为零" << std::endl;
            continue;
        }
        std::string buf = "3 " + _group + " " + _sender + ":" + _msg; 
        for (const auto& fd : it->second) {
            std::cout << fd << " ";
            send(fd, buf.c_str(), buf.size(), 0);
        }
    }
}

void chatServer::Alone_Send_thread_func() {
#ifdef TODEBUG
    std::cout << "alone send thread id = " << pthread_self() << std::endl;
#endif
    while (!m_bStop)
    {
        //std::pair<int, std::string> target_info;

        pthread_mutex_lock(&m_private_send_mutex);
        /* 注意！要用while循环等待 */
        while (private_msgs.empty())
            pthread_cond_wait(&m_private_send_cond, &m_private_send_mutex);

        auto target_info = std::move(private_msgs.front());
        private_msgs.pop();
        pthread_mutex_unlock(&m_private_send_mutex);

        while (1)
        {
            int clientfd = target_info.first;

            send(clientfd, target_info.second.c_str(), target_info.second.size(), 0);

            /* 发送完把缓冲区清干净 */
            target_info.first = -1;
            target_info.second.clear();

            if (target_info.first == -1 || target_info.second.empty())
                break;
        }

    }
}

void chatServer::Function_Send_thread_func()
{
#ifdef TODEBUG
    std::cout << "function send thread id = " << pthread_self() << std::endl;
#endif
    while (!m_bStop) {
        //std::pair<int, std::string> target_info;

        pthread_mutex_lock(&function_mutex);
        /* 注意！要用while循环等待 */
        while (function_msgs.empty())
            pthread_cond_wait(&function_cond, &function_mutex);

        auto target_info = std::move(function_msgs.front());
        function_msgs.pop();
        pthread_mutex_unlock(&function_mutex);
        while (1) {
            send(target_info.first, target_info.second.c_str(), target_info.second.size(), 0);
            /* 发送完把缓冲区清干净 */
            target_info.first = -1;
            target_info.second.clear();

            if (target_info.first == -1 || target_info.second.empty())
                break;
        }

    }
}





bool chatServer::CancleUserOnline(int clientfd) {
    std::string tmp;
    std::unordered_map<int, std::string>::iterator uit1;
    if ((uit1 = fd_onlineUser.find(clientfd)) != fd_onlineUser.end()) {
        tmp = uit1->second;
        pthread_mutex_lock(&function_mutex);
        fd_onlineUser.erase(uit1);
        pthread_mutex_unlock(&function_mutex);
        std::unordered_map<std::string, int>::iterator uit2;
        if ((uit2 = onlineUsers.find(tmp)) != onlineUsers.end()) {
            pthread_mutex_lock(&function_mutex);
            onlineUsers.erase(uit2);
            pthread_mutex_unlock(&function_mutex);

            std::string joined_groups = getJoinedGroups(uit2->first);
            std::istringstream stream(joined_groups);

            std::string word;
            while (stream >> word) {
                if (groups.find(word) != groups.end()){
                    // 检查是否存在键 "yichen"
                    auto it = groups.find(word);
                    if (it != groups.end()) {
                        // 键 "yichen" 存在，从 set 中删除值
                        it->second.erase(clientfd);
                    } else {
                        std::cout << "Key not found: " << word << std::endl;
                    }
                }

                else 
                    continue;
            }
            
        }
        return true;
    } 
    return false;
}

bool chatServer::ToRecordLogin(const std::string& username, const int& clientfd) {
    std::cout << "debug: Func ToRecordLogin username: "<<username <<"|\n";
    pthread_mutex_lock(&function_mutex);
    onlineUsers[username] = clientfd;
    fd_onlineUser[clientfd] = username;
    pthread_mutex_unlock(&function_mutex);
    std::cout << "online size: "<< this->onlineUsers.size() << std::endl;
    for (const auto& u : onlineUsers) 
        std::cout << u.first << " " << u.second << std::endl;
    std::unordered_map<std::string, int>::iterator uit = onlineUsers.find(username);
    
    if (uit == onlineUsers.end())    return false;
    
    pthread_mutex_lock(&function_mutex);
    onlineUsers[username] = clientfd;
    fd_onlineUser[clientfd] = username;
    pthread_mutex_unlock(&function_mutex);

    std::string joined_groups = getJoinedGroups(username);
    std::cout << "已加入的群聊源信息： " << joined_groups <<"\n" << std::flush;
    std::istringstream stream(joined_groups);

    std::string word;
    std::cout << "登记的用户为： "<< username << " 已加入的群聊有：" << std::flush; 
    while (stream >> word) {
        std::cout << word << " " << std::flush;
        auto it = groups.find(word);
        if (it == groups.end()) {
            // 键不存在，创建新的 set 并插入值
            std::set<int> newSet;
            std::cout << "debug before set size " << newSet.size() << std::endl;
            newSet.insert(clientfd);
            std::cout << "debug after set size " << newSet.size() << std::endl;
            groups[word] = newSet;
        } else {
            // 键存在，直接向 set 中插入值
            it->second.insert(clientfd);
        }
    }
    std::cout << std::endl;
    

    return true;
}

std::future<bool> chatServer::asyncToRecordLogin(const std::string& username, const int& clientfd) {
    return std::async(&chatServer::ToRecordLogin, this, username, clientfd);
}


std::future<bool> chatServer::asyncCancleUserOnline(int clientfd) {
    return std::async(&chatServer::CancleUserOnline, this, clientfd);
}

