#include "ReactorServer.h"
#include "command.h"


/* 登录、注册数据提取 */
static void 
getUserNameAndPass(const std::string& input, std::string& username, std::string& password) {
    std::istringstream iss(input);
    std::string command;
    iss >> command >> username >> password;
}

/* 验证用户是否存在 */
static bool 
doesUserExist(const std::string& username) {
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    // 初始化 MySQL 连接
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed" << std::endl;
        return false;
    }

    // 连接到数据库
    if (mysql_real_connect(conn, "localhost", "yichen", "1234", "chat_user", 0, nullptr, 0) == nullptr) {
        std::cerr << "mysql_real_connect() failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    // 构造查询
    std::string query = "SELECT password FROM Users WHERE username = '" + username + "';";

    // 执行查询
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "SELECT password FROM Users failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    res = mysql_store_result(conn);
    if (res == nullptr) {
        std::cerr << "mysql_store_result() failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    // 获取查询结果
    bool userExists = false;
    row = mysql_fetch_row(res);
    if (row) {
        userExists = true;
    }

    // 清理资源
    mysql_free_result(res);
    mysql_close(conn);

    return userExists;
}

/* 验证用户密码是否正确 */
static bool
verifyPassword(const std::string& username, const std::string& inputPassword) {
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    // 初始化 MySQL 连接
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed" << std::endl;
        return false;
    }

    // 连接到数据库
    if (mysql_real_connect(conn, "localhost", "yichen", "1234", "chat_user", 0, nullptr, 0) == nullptr) {
        std::cerr << "mysql_real_connect() failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    // 构造查询
    std::string query = "SELECT password FROM Users WHERE username = '" + username + "';";

    // 执行查询
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "SELECT * FROM Users failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    res = mysql_store_result(conn);
    if (res == nullptr) {
        std::cerr << "mysql_store_result() failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    // 获取查询结果
    int num_fields = mysql_num_fields(res);
    row = mysql_fetch_row(res);

    bool passwordMatches = false;
    if (row) {
        std::string dbPassword(row[0]);
        if (dbPassword == inputPassword) {
            passwordMatches = true;
        }
    }

    // 清理资源
    mysql_free_result(res);
    mysql_close(conn);

    return passwordMatches;
}

/* 注册新用户 */
static bool 
insertNewUser(const std::string& username, const std::string& password) {
    MYSQL* conn;

    // 初始化 MySQL 连接
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed" << std::endl;
        return false;
    }

    // 连接到数据库
    if (mysql_real_connect(conn, "localhost", "yichen", "1234", "chat_user", 0, nullptr, 0) == nullptr) {
        std::cerr << "mysql_real_connect() failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    // 构造插入
    std::string query = "INSERT INTO Users (`username`, `password`) VALUES ('" + username + "', '" + password + "')";

    // 执行查询
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "debug1 : " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    query.clear();
    query = "CREATE TABLE "+ username + "_friends (friends char(30))";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "debug2 : " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }
    //把自己加入到好友表中
    query.clear();
    query = "insert into "+ username +"_friends values('" + username + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "debug3 : " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }
    mysql_close(conn);

    return true;
}

static void
get_src_tar_user_msg(const std::string& input, std::string& src_username, std::string& tar_username, std::string& msg) {
    std::istringstream iss(input);
    iss >>  src_username >> tar_username >> msg;
}

static void
parse_add_friend_cmd(const std::string& cmd, std::string& request_user, std::string& target_user) {
    std::istringstream iss(cmd);
    iss >>  request_user >> target_user;
}

static std::string 
produce_add_friend_cmd(const std::string& req_user, const std::string& tar_user) {
    std::string cmd = "add_friend ";
    cmd += req_user;
    cmd += " ";
    cmd += tar_user;

    return cmd;
}

static bool
add_friend(const std::string& req_user, const std::string& tar_user) {
    MYSQL* conn;
    // 初始化 MySQL 连接
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed" << std::endl;
        return false;
    }

    // 连接到数据库
    if (mysql_real_connect(conn, "localhost", "yichen", "1234", "chat_user", 0, nullptr, 0) == nullptr) {
        std::cerr << "mysql_real_connect() failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    std::string query = "insert into "+ req_user +"_friends values('" + tar_user + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "debug : " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    mysql_close(conn);
    return true;
}

static std::string
produce_friend_verification_msg(const std::string& username) {
    std::string info = username + " has added you as a friend";
    return info;
}


static bool 
checkUserExists(const std::string& username) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // 初始化MySQL连接
    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "mysql_init() failed\n";
        return false;
    }

    // 连接到数据库
    if (mysql_real_connect(conn, "localhost", "yichen", "1234", "chat_user", 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return false;
    }

    // 构建查询语句
    std::string query = "SELECT username FROM Users WHERE username = '" + username + "'";

    // 执行查询
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "SELECT query failed: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return false;
    }

    // 获取查询结果
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "mysql_store_result() failed: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return false;
    }

    // 检查是否有结果行
    bool userExists = false;
    if ((row = mysql_fetch_row(res)) != NULL) {
        userExists = true;
    }

    // 释放结果集
    mysql_free_result(res);

    // 关闭数据库连接
    mysql_close(conn);

    return userExists;
}

/* 检查是否是好友 */
static bool 
checkFrinedExists(const std::string& srcname, const std::string& tarname) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // 初始化MySQL连接
    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "mysql_init() failed\n";
        return false;
    }

    // 连接到数据库
    if (mysql_real_connect(conn, "localhost", "yichen", "1234", "chat_user", 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return false;
    }

    // 构建查询语句
    std::string query = "SELECT friends FROM " + srcname + "_friends WHERE friends = '" + tarname + "'";

    // 执行查询
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "SELECT query failed: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return false;
    }

    // 获取查询结果
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "mysql_store_result() failed: " << mysql_error(conn) << "\n";
        mysql_close(conn);
        return false;
    }

    // 检查是否有结果行
    bool friendExists = false;
    if ((row = mysql_fetch_row(res)) != NULL) {
        friendExists = true;
    }

    // 释放结果集
    mysql_free_result(res);

    // 关闭数据库连接
    mysql_close(conn);

    return friendExists;
}


Reactor_Server::Reactor_Server() {}
Reactor_Server::~Reactor_Server() {}

struct ARG {
    Reactor_Server* pThis;
};


bool
Reactor_Server::Init(const char* ip, unsigned short port) {

    if (!Create_server_listener(ip, port)) {
        std::cout << "Unable to bind:" << ip << ":" << port << "." << std::endl;
        return false;
    }

    std::cout << "main thread id = " << pthread_self() << " port: "<< port << std::endl;

    ARG* arg = new ARG();
    arg->pThis = this;

    pthread_create(&m_accept_threadid, NULL, Accept_thread_func, (void*)arg);

    for (int i = 0; i < thread_number; i++) {
        pthread_create(&m_threadid[i], NULL, Worker_thread_func, (void*)arg);
    }

    for (int i = 0; i < thread_number; i++) {
        pthread_create(&m_group_send_threadid[i], NULL, Group_Send_thread_func, (void*)arg);
    }

    for (int i = 0; i < thread_number; i++) {
        pthread_create(&m_alone_send_threadid[i], NULL, Alone_Send_thread_func, (void*)arg);
    }

    return true;
}


bool 
Reactor_Server::Uninit() {
    m_bStop = true;

    /* 读端和写端都关闭 */
    shutdown(m_listenfd, SHUT_RDWR);
    close(m_listenfd);
    close(m_epollfd);


    return true;
}

void* 
Reactor_Server::Main_loop(void* p) {
    std::cout << "main thread id = " << pthread_self() << std::endl;

    Reactor_Server* pReactor = static_cast<Reactor_Server*>(p);

    while (!pReactor->m_bStop) {
        /* std::cout << "main loop" << std::endl; */
        struct epoll_event ev[1024];
        int n = epoll_wait(pReactor->m_epollfd, ev, 1024, 10);
        if(n == 0)
            continue;
        else if(n < 0) {
            std::cout << "epoll_wait error" << std::endl;
            continue;
        }

        int m = std::min(n, 1024);
        for (int i = 0; i < m; i++) {
            /* 有新连接 */
            if (ev[i].data.fd == pReactor->m_listenfd)
                pthread_cond_signal(&pReactor->m_accept_cond);
            /* 有数据 */
            else {
                pthread_mutex_lock(&pReactor->m_client_mutex);
                pReactor->m_clientqueue.push(ev[i].data.fd);
                pthread_mutex_unlock(&pReactor->m_client_mutex);

                pthread_cond_signal(&pReactor->m_client_cond);
            }
        }
    }

    std::cout << "main loop exit ..." << std::endl;
    return NULL;
}


bool 
Reactor_Server::Close_client(int clientfd, void* arg) {
    Reactor_Server* pReactor = static_cast<Reactor_Server*>(arg);

#ifdef TODEBUG
        std::cout << "before set size: " << pReactor->m_fds.size() << std::endl;
        std::cout << "before online size: " << pReactor->fd_onlineUser.size() << std::endl;
        std::cout << "before online size: " << pReactor->onlineUsers.size() << std::endl;
#endif

#ifdef TODEBUG
    std::cout << "client fd: "<< clientfd << "disconnected" << std::endl;
#endif
    if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, clientfd, NULL) == -1) {
        std::cout << "release client socket failed as call epoll_ctl fail" << std::endl;
    }

    std::set<int>::iterator sit;
    if ((sit = pReactor->m_fds.find(clientfd)) != pReactor->m_fds.end()) {
        pReactor->m_fds.erase(sit);
    }

    std::string tmp;
    std::unordered_map<int, std::string>::iterator uit1;
    if ((uit1 = pReactor->fd_onlineUser.find(clientfd)) != pReactor->fd_onlineUser.end()) {
        tmp = uit1->second;
        pReactor->fd_onlineUser.erase(uit1);
    }

    std::unordered_map<std::string, int>::iterator uit2;
    if ((uit2 = pReactor->onlineUsers.find(tmp)) != pReactor->onlineUsers.end()) {
        pReactor->onlineUsers.erase(uit2);
    }

#ifdef TODEBUG
        std::cout << "now set size: " << pReactor->m_fds.size() << std::endl;
        std::cout << "now online size: " << pReactor->fd_onlineUser.size() << std::endl;
        std::cout << "now online size: " << pReactor->onlineUsers.size() << std::endl;
#endif

    close(clientfd);
    return true;
}


bool 
Reactor_Server::Create_server_listener(const char* ip, unsigned short port) {
    m_listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (m_listenfd == -1) {
        return false;
    }

    /* 端口地址复用 */
    int on = 1;
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof(on));

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);
 
    if (bind(m_listenfd, (sockaddr*)&servaddr, sizeof(servaddr)) == -1)
        return false;

    if (listen(m_listenfd, 50) == -1)
        return false;

    m_epollfd = epoll_create(1);
    if (m_epollfd == -1)
        return false;

    struct epoll_event e;
    memset(&e, 0, sizeof(e));
    e.events = EPOLLIN | EPOLLRDHUP;
    e.data.fd = m_listenfd;
    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_listenfd, &e) == -1)
        return false;

    return true;
}


void* 
Reactor_Server::Accept_thread_func(void* args) {
    ARG* arg = static_cast<ARG*>(args);
    Reactor_Server* pReactor = arg->pThis;

    while (!pReactor->m_bStop) {
        pthread_mutex_lock(&pReactor->m_accept_mutex);
        pthread_cond_wait(&pReactor->m_accept_cond, &pReactor->m_accept_mutex);

        struct sockaddr_in clientaddr;
        socklen_t addrlen;
        int newfd = accept(pReactor->m_listenfd, (struct sockaddr*)&clientaddr, &addrlen);
        pthread_mutex_unlock(&pReactor->m_accept_mutex);
        if (newfd == -1)
            continue;

#if 1
        pthread_mutex_lock(&pReactor->m_cli_mutex);
        pReactor->m_fds.insert(newfd);
        pthread_mutex_unlock(&pReactor->m_cli_mutex);
#endif

#ifdef TODEBUG
        std::cout << "new client connected: " << newfd << std::endl;
#endif

        /* 将新套接字设置为非阻塞式 */
        int oldflag = fcntl(newfd, F_GETFL, 0);
        int newflag = oldflag | O_NONBLOCK;
        if (fcntl(newfd, F_SETFL, newflag) == -1) {
#ifdef TODEBUG
            std::cout << "fcntl error, oldflag = " << oldflag << ", newflag = " << newflag << std::endl;
#endif
            continue;
        }

        struct epoll_event e;
        memset(&e, 0, sizeof(e));
        e.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
        e.data.fd = newfd;
        /* 注册epoll的兴趣列表 */
        if (epoll_ctl(pReactor->m_epollfd, EPOLL_CTL_ADD, newfd, &e) == -1) {
#ifdef TODEBUG
            std::cout << "epoll_ctl error, fd = " << newfd << std::endl;
#endif
        } else {
#ifdef TODEBUG
            std::cout << "epoll_ctl success, fd = " << newfd << std::endl;
#endif
        }   
    }

    return NULL;
}



void* 
Reactor_Server::Worker_thread_func(void* args) {
    ARG* arg = static_cast<ARG*>(args);
    Reactor_Server* pReactor = arg->pThis;

    while (!pReactor->m_bStop) {
        int solution;
        int clientfd;
        pthread_mutex_lock(&pReactor->m_client_mutex);
        /* 注意！要用while循环等待 */
        while(pReactor->m_clientqueue.empty())
            pthread_cond_wait(&pReactor->m_client_cond, &pReactor->m_client_mutex);

        /* 取出客户套接字 */
        clientfd = pReactor->m_clientqueue.front();
        pReactor->m_clientqueue.pop();
        pthread_mutex_unlock(&pReactor->m_client_mutex);

        // std::cout << std::endl;

        time_t now = time(NULL);
        struct tm* nowstr = localtime(&now);
        std::ostringstream ostimestr;

        std::string strclientmsg;
        std::pair<int, std::string> add_friend_info;
        std::pair<int, std::string> add_friend_info2;
        std::pair<int, std::string> msg_taruserfd;
        char buff[256];
        bool bError = false;
        // std::cout << "wait...."<< std::endl;
        while (true) {
            memset(buff, 0, sizeof(buff));
            int nRecv = recv(clientfd, buff, 256, 0);

            /* recv error */
            if (nRecv == -1) {
                if(errno == EWOULDBLOCK)
                    break;
                else {
                    pReactor->Close_client(clientfd, static_cast<void*>(pReactor));
                    bError = true;
                    break;
                }
            }
            /* client close */
            else if (nRecv == 0) {
                /* 将该客户从客户列表中删除 */
                pthread_mutex_lock(&pReactor->m_cli_mutex);
                pReactor->m_fds.erase(clientfd);
                pthread_mutex_unlock(&pReactor->m_cli_mutex);

                pReactor->Close_client(clientfd, static_cast<void*>(pReactor));
                bError = true;
                break;
            }

            std::string input(buff);
            if (buff[0] == REGISTER) { /* 注册 */
                solution = 0;
                std::string username; 
                std::string password;
                getUserNameAndPass(input, username, password);
                if (doesUserExist(username)) {
                    //send(clientfd, "The name has been registered", sizeof("The name has been registered"), 0);
                    send(clientfd, "1", sizeof("1"), 0);
                    continue;
                } else {
                    if (insertNewUser(username, password)) {
                        //send(clientfd, "register success", sizeof("register success"), 0);
                        send(clientfd, "0", sizeof("0"), 0);
                        continue;
                    } else {
                        send(clientfd, "server error", sizeof("server error"), 0);
                        continue;
                    }
                }
            } else if (buff[0] == LOGIN) { /* 登录 */
                solution = 1;
                std::string username; 
                std::string password;
                getUserNameAndPass(input, username, password);
                if (!doesUserExist(username)) {
                    //send(clientfd, "user doesn't exist", sizeof("user doesn't exist"), 0);
                    send(clientfd, "0", sizeof("0"), 0);
                    continue;
                } else {
                    if (verifyPassword(username, password)) {
                        if (pReactor->onlineUsers.find(username) != pReactor->onlineUsers.end()) {
                            //send(clientfd, "the user has logged in elsewhere", sizeof("the user has logged in elsewhere"), 0);
                            send(clientfd, "1", sizeof("1"), 0);
                            continue;
                        } else {
                            //std::string tmp("welcome back ");
                            //tmp += username;
                            pthread_mutex_lock(&pReactor->m_cli_mutex);
                            pReactor->onlineUsers[username] = clientfd;
                            pReactor->fd_onlineUser[clientfd] = username;
                            pthread_mutex_unlock(&pReactor->m_cli_mutex);
                            //send(clientfd, tmp.c_str(), tmp.size(), 0);
                            send(clientfd, "2", sizeof("2"), 0);
                            continue;
                        }
                        
                    } else {
                        //send(clientfd, "password error", sizeof("password error"), 0);
                        send(clientfd, "3", sizeof("3"), 0);
                        continue;
                    }
                }
                
            } else if (buff[0] == GMESSAGE) { /* 群消息 */
                /* TODO */
                solution = 3;
                input.erase(input.begin());
                input.erase(input.begin());
                strclientmsg = input;
            } else if (buff[0] == ADDFRIEND) { /* 添加好友 */
                /* TODO */
                solution = 4;
                input.erase(input.begin());
                input.erase(input.begin());
                std::string req_user;
                std::string tar_user;
                parse_add_friend_cmd(input, req_user, tar_user);

#ifdef TODEBUG
                std::cout << "add friend " << "src:" << req_user << " tar:"<< tar_user << std::endl;
#endif
                
                if (pReactor->onlineUsers.find(req_user) == pReactor->onlineUsers.end()) {
                    send(clientfd, "please log in first", sizeof("please log in first"), 0);
                    pReactor->Close_client(clientfd, static_cast<void*>(pReactor));
                    continue;
                }

                
                if (checkUserExists(tar_user)) {
                    if (checkFrinedExists(req_user, tar_user)) {
                        send(clientfd, "He is already your friend", sizeof("He is already your friend"), 0);
                        continue;
                    }
                    if (pReactor->onlineUsers.find(tar_user) == pReactor->onlineUsers.end()) {
                        send(clientfd, "The user not online", sizeof("The user not online"), 0);
                        continue;
                    } else {
                        add_friend_info.first = pReactor->onlineUsers[tar_user];
                        add_friend_info.second = produce_add_friend_cmd(req_user, tar_user);
                    }
                } else {
                    send(clientfd, "The user you added does not exist", sizeof("The user you added does not exist"), 0);
                    continue;
                }
            } else if (buff[0] == PMESSAGE) { /* 一对一消息 */
                /* TODO */
                solution = 5;
                input.erase(input.begin());
                input.erase(input.begin());
                std::string src_user;
                std::string tar_user;
                std::string msg;
                get_src_tar_user_msg(input, src_user, tar_user, msg);
                if (checkFrinedExists(src_user, tar_user)) {
                    std::unordered_map<std::string, int>::const_iterator cit;
                    if ((cit = pReactor->onlineUsers.find(tar_user)) != pReactor->onlineUsers.end()) {
                        msg_taruserfd.first = cit->second;
                        msg_taruserfd.second = msg;
                    } else {
                        send(clientfd, "He/She is not online", sizeof("He/She is not online"), 0);
                        continue;
                    }
                } else {
                    send(clientfd, "He/She is not your friend", sizeof("He/She is not your friend"), 0);
                    continue;
                }
            } else if (buff[0] == FRIENDREPLY) { /* 好友申请回复 */
                solution = 6;
                input.erase(input.begin());
                input.erase(input.begin());
                std::string src_user;
                std::string tar_user;
                std::string msg;
                get_src_tar_user_msg(input, src_user, tar_user, msg);
                if (checkUserExists(src_user) && checkUserExists(tar_user)) {
                    if (add_friend(src_user, tar_user) && add_friend(tar_user, src_user) && 
                    pReactor->onlineUsers.find(src_user) != pReactor->onlineUsers.end() &&
                    pReactor->onlineUsers.find(tar_user) != pReactor->onlineUsers.end()) {
                        add_friend_info.first = pReactor->onlineUsers[tar_user];
                        add_friend_info2.first = pReactor->onlineUsers[src_user];
                        add_friend_info.second = produce_friend_verification_msg(src_user);
                        add_friend_info2.second = produce_friend_verification_msg(tar_user);
                    }
                }
            }
        }

        /* 如果出错了就不必往下执行了 */
        if (bError) {
            continue;
        }


        /* 将消息交给发送消息的线程 */
        if (solution == 3) {
            pReactor->m_group_msgs.push_back(strclientmsg);
        } else if (solution == 4) {
            pReactor->m_alone_msgs.push(add_friend_info);
        } else if (solution == 5) {
            pReactor->m_alone_msgs.push(msg_taruserfd);
        } else if (solution == 6) {
            pReactor->m_alone_msgs.push(add_friend_info);
            pReactor->m_alone_msgs.push(add_friend_info2);
        }
        pthread_cond_signal(&pReactor->m_send_cond);
        
    }
    return NULL;
}


void* 
Reactor_Server::Group_Send_thread_func(void* args) {
    ARG* arg = static_cast<ARG*>(args);
    Reactor_Server* pReactor = arg->pThis;

    while (!pReactor->m_bStop) {
        std::string strclientmsg;

        pthread_mutex_lock(&pReactor->m_send_mutex);
        /* 注意！要用while循环等待 */
        while(pReactor->m_group_msgs.empty())
            pthread_cond_wait(&pReactor->m_send_cond, &pReactor->m_send_mutex);

        strclientmsg = pReactor->m_group_msgs.front();
        pReactor->m_group_msgs.pop_front();
        pthread_mutex_unlock(&pReactor->m_send_mutex);


        while(1) {
            int nSend;
            int clientfd;
            /* 广播消息 */
            for (auto it = pReactor->m_fds.begin(); it != pReactor->m_fds.end(); it++) {
                clientfd = *it;
                nSend = send(clientfd, strclientmsg.c_str(), strclientmsg.length(), 0);
                if(nSend == -1) {
                    if(errno == EWOULDBLOCK) {
                        sleep(10);
                        continue;
                    } else {
                        pReactor->Close_client(clientfd, static_cast<void*>(pReactor));
                        break;
                    }
                }
            }

#ifdef TODEBUG
            std::cout << "server send:" << strclientmsg << std::endl;
#endif
            /* 发送完把缓冲区清干净 */
            strclientmsg.clear();

            if(strclientmsg.empty())
                break;
        }
    }

    return NULL;
}


void* 
Reactor_Server::Alone_Send_thread_func(void* args) {
    ARG* arg = static_cast<ARG*>(args);
    Reactor_Server* pReactor = arg->pThis;

    while (!pReactor->m_bStop) {
        std::pair<int, std::string> target_info;

        pthread_mutex_lock(&pReactor->m_send_mutex);
        /* 注意！要用while循环等待 */
        while(pReactor->m_alone_msgs.empty())
            pthread_cond_wait(&pReactor->m_send_cond, &pReactor->m_send_mutex);

        target_info = pReactor->m_alone_msgs.front();
        pReactor->m_alone_msgs.pop();
        pthread_mutex_unlock(&pReactor->m_send_mutex);


        while(1) {
            int clientfd = target_info.first;

            send(clientfd, target_info.second.c_str(), target_info.second.size(), 0);
            

#ifdef TODEBUG
            std::cout << "server send to fd : "  << target_info.second << std::endl;
#endif
            /* 发送完把缓冲区清干净 */
            target_info.first = -1;
            target_info.second.clear();

            if(target_info.first == -1 || target_info.second.empty())
                break;
        }
    }

    return NULL;
}




