#ifndef CHAT_SERVER_V2_H_
#define CHAT_SERVER_V2_H_

/* a simple reactor server(foundtion:mysql) */

#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     /* signal() */
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>   /* pthread_mutex_t pthread_cond_t */
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>  /* htonl()、htons() */
#include <sys/epoll.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <queue>
#include <set>
#include <deque>
#include <vector>
#include <memory>
#include <iomanip>     /* std::setw()/setfill() */
#include <sstream>
#include <iostream>

#include <mysql/mysql.h>



constexpr int work_thread_number = 4;


class Reactor_Server {
public:
    Reactor_Server();
    ~Reactor_Server();

    //inline bool getStop(){ return m_bStop; }

    /* 初始化socket和线程 */
    bool Init(const char* ip, unsigned short port = 9999);

    bool Uninit();

    bool Close_client(int clientfd);

    static void* Main_loop(void* loop);


private:
    /* 禁止拷贝构造、赋值及移动构造 */
    Reactor_Server(const Reactor_Server& rhs) = delete;
    Reactor_Server(Reactor_Server&& rhs) = delete;
    Reactor_Server& operator=(const Reactor_Server& rhs) = delete;
    Reactor_Server& operator=(Reactor_Server&& rhs) = delete;

    static void* Accept_thread_func(void* args);
    static void* Worker_thread_func(void* args);
    static void* Send_thread_func(void* args);

    bool Create_server_listener(const char* ip, unsigned short port = 9999);

private:

    /* 服务器端的socket */
    int m_listenfd = 0;
    /* 让线程可以修改它 */
    int m_epollfd = 0;
    /* 决定主线程、accept线程、工作线程是否继续迭代 */
    bool m_bStop = false;

    /* 线程ID */
    pthread_t m_accept_threadid;
    pthread_t m_threadid[work_thread_number];

    pthread_t m_send_threadid;

    /* 接受客户的信号量 */
    pthread_mutex_t m_accept_mutex = PTHREAD_MUTEX_INITIALIZER;
    /* 有新连接的条件变量 */
    pthread_cond_t m_accept_cond = PTHREAD_COND_INITIALIZER;

    /* 添加、取出客户链表的信号量 */
    pthread_mutex_t m_client_mutex = PTHREAD_MUTEX_INITIALIZER;
    /* 通知工作线程有客户消息的条件变量 */
    pthread_cond_t m_client_cond = PTHREAD_COND_INITIALIZER;

    pthread_mutex_t m_send_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t m_send_cond = PTHREAD_COND_INITIALIZER;

    pthread_mutex_t m_cli_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t m_cli_cond = PTHREAD_COND_INITIALIZER;

    std::set<int> m_fds;
    /* 客户端套接字队列 */
    std::queue<int> m_clientqueue;
    /* 伪消息队列 */
    std::deque<std::string> m_msgs;
};


#endif