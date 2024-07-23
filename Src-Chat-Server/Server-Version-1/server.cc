/**
 * @file server.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-07-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <netinet/in.h> /* sockaddr_in */
#include <sys/socket.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/shm.h> /* share memory */
#include <time.h>
#include <sys/time.h> /* gettimeofday */


using namespace std;

static constexpr int main_ret = 0;
static constexpr int queuelong = 20;
const char* welcome = "\n|---------------Welcome to the Chat Room!----------------|\n|\t\tdeveloper:yichen\t\t |\n";

#define DEBUG(expr, msg)  if (expr) { perror(static_cast<const char*>(msg)); exit(1); }

// 时间戳
static void 
get_cur_time(char* time_str) {
    struct timeval now;
    gettimeofday(&now, NULL);
    strcpy(time_str, ctime(&now.tv_sec));
}

static int
SocketInit(int port = 8888) {
    int sockfd;
    struct sockaddr_in my_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET; 
    my_addr.sin_port = htons(port);         // 字节序
    my_addr.sin_addr.s_addr = INADDR_ANY;

    DEBUG(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1, "fail to bind");
    DEBUG(listen(sockfd, queuelong) == -1, "fail to listen");
    return sockfd;
}


// 创建共享存储区
static int 
ShmCreate() {
    int shmid;
    if ((shmid = shmget(IPC_PRIVATE, 1024, S_IRUSR | S_IWUSR)) == -1) { /*S_IRUSR | S_IWUSR 表示用户可读可写*/
        fprintf(stderr, "Create Share Memory Error:%s\n\a", strerror(errno));
        exit(1);
    }
    return shmid;
}

int main(int argc, char** argv) {
    pid_t pid;
    pid_t ppid;
    int clientfd;
    char* buf;
    char* read_addr;
    char* write_addr;
    ssize_t recvbytes;
    struct sockaddr_in their_addr;
    socklen_t sin_size = sizeof(sockaddr_in);
    int sockfd = SocketInit();
    int shmid = ShmCreate();
    char* temp = (char *)malloc(255);
    char* time_str = (char *)malloc(50);
    get_cur_time(time_str);
    printf("Server working current time is : %s\n", time_str);

    while (1) {
        //接受一个客户端的连接请求,相当于不限客户数，群发消息
        if ((clientfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("fail to accept");
            exit(1);
        }
        
        //得到客户端的IP地址输出
        char address[20];
        inet_ntop(AF_INET, &their_addr.sin_addr, address, sizeof(address));

        
        send(clientfd, welcome, strlen(welcome), 0); //发送问候信息
        buf = (char*)malloc(255);

        ppid = fork(); //创建子进程
        if (ppid == 0) { //子进程 
            pid = fork(); //子进程创建子进程
            while (1)
            {
                if (pid > 0) {
                    //父进程用于接收信息
                    memset(buf, 0, 255);
                    if ((recvbytes = recv(clientfd, buf, 255, 0)) <= 0) {
                        perror("fail to recv");
                        close(clientfd);
                        raise(SIGKILL);
                        exit(1);
                    }
                    //shmat将shmid所代表的全局的共享存储区关联到本进程的进程空间
                    write_addr = static_cast<char*>(shmat(shmid, 0, 0)); 
                    memset(write_addr, '\0', 1024);

                    //把接收到的消息存入共享存储区中
                    strncpy(write_addr, buf, 1024);

                    //把接收到的消息连接此刻的时间字符串输出到标准输出
                    get_cur_time(time_str);
                    strcat(buf, time_str);
                    printf("%s\n", buf);
                }
                else if (pid == 0) {
                    //子进程的子进程用于发送消息
                    sleep(1); //子进程先等待父进程把接收到的信息存入共享存储区
                    read_addr = static_cast<char*>(shmat(shmid, 0, 0)); //读取共享存储区的内容

                    //temp存储上次读取过的内容,每次先判断是否已经读取过该消息
                    if (strcmp(temp, read_addr) != 0) {
                        strcpy(temp, read_addr); //更新temp，表示已经读取过该消息

                        get_cur_time(time_str);
                        strcat(read_addr, "  ");
                        strcat(read_addr, time_str);
                        if (send(clientfd, read_addr, strlen(read_addr), 0) == -1) {
                            perror("fail to send");
                            exit(1);
                        }
                        memset(read_addr, '\0', 1024);
                        strcpy(read_addr, temp);
                    }
                }
                else
                    perror("fail to fork");
            }
        }
        
        free(buf);
    }

    
    free(temp);
    free(time_str);

    return main_ret;
}
