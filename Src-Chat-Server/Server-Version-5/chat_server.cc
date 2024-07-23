#include "command.h"
#include "functions.h"
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
    for (int i = 0; i < thread_number; ++i) 
        std::thread(&chatServer::Worker_thread_func, this).detach();
    
    for (int i = 0; i < thread_number; ++i) {
        std::thread(&chatServer::Function_Send_thread_func, this).detach();
        std::thread(&chatServer::Alone_Send_thread_func, this).detach();
    }
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
    removeSocketFromEpoll(m_epollfd, clientfd);
    CancleUserOnline(clientfd);
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
                ClientClose(clientfd);
                break;
            }

            std::string input;
            input.clear();
            input = buff;
            std::string tmp;

            switch (buff[0]) {
                case REGISTER: { /* 注册 */
                    std::string username;
                    std::string password;
                    getUserNameAndPass(input, username, password);
                    if (doesUserExist(username)) {
                        send(clientfd, "1", sizeof("1"), 0);
                    } else {
                        if (insertNewUser(username, password)) {
                            send(clientfd, "0", sizeof("0"), 0);
                        } else {
                            //send(clientfd, "server error", sizeof("server error"), 0);
                            send(clientfd, "2", sizeof("0"), 0);
                        }
                    }
                    break;
                }
                case LOGIN: { /* 登录 */
                    std::string username;
                    std::string password;
                    getUserNameAndPass(input, username, password);
                    if (!doesUserExist(username))
                    {
                        send(clientfd, "0", sizeof("0"), 0);
                    }
                    else
                    {
                        if (verifyPassword(username, password))
                        {
                            if (onlineUsers.find(username) != onlineUsers.end())
                            {
                                send(clientfd, "1", sizeof("1"), 0);
                            }
                            else
                            {
                                pthread_mutex_lock(&function_mutex);
                                onlineUsers[username] = clientfd;
                                fd_onlineUser[clientfd] = username;
                                pthread_mutex_unlock(&function_mutex);
                                send(clientfd, "2", sizeof("2"), 0);
                            }
                        }
                        else
                        {
                            send(clientfd, "3", sizeof("3"), 0);
                        }
                    }
                    break;
                }
                case GMESSAGE: { /* 群消息 */
                    input.erase(input.begin(), input.begin() + 2);
                    /* TODO */
                    break;
                }
                case ADDFRIEND: { /* 添加好友 */
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
                case PMESSAGE: { /* 一对一消息 */
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
                case FRIENDREPLY: { /* 好友申请回复 */
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
                case GETUSERFRIENDS: { /* 获取所有好友用户名 */
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
                case UPDATESOCKET: {
                    input.erase(input.begin(), input.begin() + 2);
                    UserChangeSocketfd(input, clientfd);
                    break;
                }
                default: {
                    // 处理未识别的命令
                    break;
                }
            }
        }
    }


}


void chatServer::Group_Send_thread_func()
{
    /* TODO */

    while (!m_bStop) {
        std::string strclientmsg;

        pthread_mutex_lock(&m_group_send_mutex);
        /* 注意！要用while循环等待 */
        while (group_msgs.empty())
            pthread_cond_wait(&m_group_send_cond, &m_group_send_mutex);

        auto msg = std::move(group_msgs.front());
        group_msgs.pop();
        pthread_mutex_unlock(&m_group_send_mutex);

        while (1)
        {
            int nSend;
            int clientfd;
            /* 广播消息 */
#if 0
            for (auto it = m_fds.begin(); it != m_fds.end(); it++)
            {
                clientfd = *it;
                nSend = send(clientfd, strclientmsg.c_str(), strclientmsg.length(), 0);
                if (nSend == -1)
                {
                    if (errno == EWOULDBLOCK)
                    {
                        sleep(10);
                        continue;
                    }
                    else
                    {
                        ClientClose(clientfd);
                        break;
                    }
                }
            }
#endif
            /* 发送完把缓冲区清干净 */
            strclientmsg.clear();

            if (strclientmsg.empty())
                break;
        }
    }
}

void chatServer::Alone_Send_thread_func() {
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