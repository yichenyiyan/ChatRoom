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

    pthread_create(&m_send_threadid, NULL, Send_thread_func, (void*)arg);

    std::cout << "accept thread " << std::endl;

    for (int i = 0; i < work_thread_number; i++) {
        pthread_create(&m_threadid[i], NULL, Worker_thread_func, (void*)arg);
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
Reactor_Server::Main_loop(void *p) {
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
Reactor_Server::Close_client(int clientfd) {
#ifdef TODEBUG
    std::cout << "client fd: "<< clientfd << "disconnected" << std::endl;
#endif
    if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, clientfd, NULL) == -1) {
        std::cout << "release client socket failed as call epoll_ctl fail" << std::endl;
    }

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
 
    if (bind(m_listenfd, (sockaddr *)&servaddr, sizeof(servaddr)) == -1)
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


        pthread_mutex_lock(&pReactor->m_cli_mutex);
        pReactor->m_fds.insert(newfd);
        pthread_mutex_unlock(&pReactor->m_cli_mutex);

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
        int clientfd;
        pthread_mutex_lock(&pReactor->m_client_mutex);
        /* 注意！要用while循环等待 */
        while(pReactor->m_clientqueue.empty())
            pthread_cond_wait(&pReactor->m_client_cond, &pReactor->m_client_mutex);

        /* 取出客户套接字 */
        clientfd = pReactor->m_clientqueue.front();
        pReactor->m_clientqueue.pop();
        pthread_mutex_unlock(&pReactor->m_client_mutex);

        std::cout << std::endl;

        time_t now = time(NULL);
        struct tm* nowstr = localtime(&now);
        std::ostringstream ostimestr;

        std::string strclientmsg;
        char buff[256];
        bool bError = false;
        while (true) {
            memset(buff, 0, sizeof(buff));
            int nRecv = recv(clientfd, buff, 256, 0);

            /* recv error */
            if (nRecv == -1) {
                if(errno == EWOULDBLOCK)
                    break;
                else {
                    pReactor->Close_client(clientfd);
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

                pReactor->Close_client(clientfd);
                bError = true;
                break;
            }

            std::string input(buff);
            if (buff[0] == REGISTER) { /* 注册 */
                std::string username; 
                std::string password;
                getUserNameAndPass(input, username, password);
                if (doesUserExist(username)) {
                    strclientmsg += "The name has been registered";
                } else {
                    if (insertNewUser(username, password)) {
                        strclientmsg += "register success";
                    } else {
                        strclientmsg += "server error";
                    }
                }
                

            } else if (buff[0] == LOGIN) { /* 登录 */
                std::string username; 
                std::string password;
                getUserNameAndPass(input, username, password);
                if (!doesUserExist(username)) {
                    strclientmsg += "user doesn't exist";
                } else {
                    if (verifyPassword(username, password)) {
                        strclientmsg += "welcome back " + username;
                    } else {
                        strclientmsg += "password error";
                    }
                }
                
            } else if (buff[0] == MESSAGE) { /* 消息 */
                /* TODO */
                strclientmsg += buff;
            } else if (buff[0] == ADDFRIEND) { /* 添加好友 */
                /* TODO */

            }
        }

        /* 如果出错了就不必往下执行了 */
        if (bError) {
            continue;
        }

#if 0
        if(strclientmsg[0] == MESSAGE)
        {
            strclientmsg.erase(0, 1); //将命令标识符去掉
            /* 将消息加上时间戳 */
            ostimestr << "[" << nowstr->tm_year + 1900 << "-"
                << std::setw(2) << std::setfill('0') << nowstr->tm_mon + 1 << "-"
                << std::setw(2) << std::setfill('0') << nowstr->tm_mday << " "
                << std::setw(2) << std::setfill('0') << nowstr->tm_hour << ":"
                << std::setw(2) << std::setfill('0') << nowstr->tm_min << ":"
                << std::setw(2) << std::setfill('0') << nowstr->tm_sec << " ] ";

            strclientmsg.insert(0, ostimestr.str());
        } else {
            send(clientfd, strclientmsg.c_str(), strclientmsg.length(), 0);
            continue;
        }
#endif


        /* 将消息交给发送消息的线程 */
        pReactor->m_msgs.push_back(strclientmsg);
        pthread_cond_signal(&pReactor->m_send_cond);
    }
    return NULL;
}


void* 
Reactor_Server::Send_thread_func(void* args) {
    ARG* arg = static_cast<ARG*>(args);
    Reactor_Server* pReactor = arg->pThis;

    while (!pReactor->m_bStop) {
        std::string strclientmsg;

        pthread_mutex_lock(&pReactor->m_send_mutex);
        /* 注意！要用while循环等待 */
        while(pReactor->m_msgs.empty())
            pthread_cond_wait(&pReactor->m_send_cond, &pReactor->m_send_mutex);

        strclientmsg = pReactor->m_msgs.front();
        pReactor->m_msgs.pop_front();
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
                    }
                    else {
                        //my_logger->error("send error, fd = {}", clientfd);
                        pReactor->Close_client(clientfd);
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



// 发送消息函数
void handleMessage(const std::string& input) {
    std::istringstream iss(input);
    std::string command, sessionToken;
    std::string message;
    iss >> command >> sessionToken;
    
}
