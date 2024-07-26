#ifndef CHAT_SERVER_H_
#define CHAT_SERVER_H_

#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     /* signal() */
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>  /* htonl()、htons() */
#include <sys/epoll.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <future>
#include <thread>
#include <set>
#include <queue>
#include <list>
#include <deque>
#include <vector>
#include <memory>
#include <iomanip>     /* std::setw()/setfill() */
#include <iostream>
#include <unordered_map>


#include "functions.h"


/* yichen chatRoom designed by a Reactor Server! */

class chatServer {
public:
    chatServer(const char* ip = "0.0.0.0", unsigned short port = 8888);
    ~chatServer();

    void main_loop();
    bool Quit();

    bool ClientClose(int clientfd);


private:
    /* 禁止拷贝构造、赋值及移动构造 */
    chatServer(const chatServer& rhs) = delete;
    chatServer(chatServer&& rhs) = delete;
    chatServer& operator=(const chatServer& rhs) = delete;
    chatServer& operator=(chatServer&& rhs) = delete;

    
    void Accept_thread_func();
    void Worker_thread_func();
    void Group_Send_thread_func();
    void Alone_Send_thread_func();
    void Function_Send_thread_func();

    std::future<bool> asyncToRecordLogin(const std::string& username, const int& clientfd);
    bool ToRecordLogin(const std::string& username, const int& clientfd);
    bool CancleUserOnline(int clientfd);
    std::future<bool> asyncCancleUserOnline(int clientfd);
    std::future<bool> asyncUserChangeSocketfd(const std::string& userName, const int& fd);
    

private:
    bool   m_bStop;
    int  m_epollfd;
    int m_listenfd;

    pthread_mutex_t m_accept_mutex;
    pthread_cond_t m_accept_cond;

    pthread_mutex_t m_worker_mutex;
    pthread_cond_t m_worker_cond;

    pthread_mutex_t m_group_send_mutex;
    pthread_cond_t m_group_send_cond;

    pthread_mutex_t m_private_send_mutex;
    pthread_cond_t m_private_send_cond;

    pthread_mutex_t function_mutex;
    pthread_cond_t function_cond;


private:
    std::queue<int> new_event_fds;
    /* 处理来自客户的消息的数据结构 */
    std::unordered_map<std::string, std::set<int>> group_users; /* 群聊名称 在线群聊成员套接字 */
    std::queue<std::pair<std::string, std::pair<std::string, std::string>>> group_msgs;    /* 群聊 (发送用户 消息内容) */
    std::queue<std::pair<int, std::string>> private_msgs;
    std::queue<std::pair<int, std::string>> function_msgs;
    /* 处理发送给客户的消息的数据结构 */

private:
    /* 在线用户映射表 */
    std::unordered_map<std::string, int> onlineUsers;
    /* 辅助性映射表 */
    std::unordered_map<int, std::string> fd_onlineUser;
    /* 群聊映射表 */
    std::unordered_map<std::string, std::set<int>> groups;


private:
    

    inline void AddOnlineUser(std::string name, int fd) {
        pthread_mutex_lock(&function_mutex);
        onlineUsers[name] = fd;
        fd_onlineUser[fd] = name;
        pthread_mutex_unlock(&function_mutex);
    }

};


#endif