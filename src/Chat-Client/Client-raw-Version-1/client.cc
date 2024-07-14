/**
 * @file client.cc
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
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

using namespace std;

static constexpr int main_ret = 0;

#define DEBUG(expr, msg)  if (expr) { perror(static_cast<const char*>(msg)); exit(1); }


int main(int argc, char** argv) {
    if (argc != 4) {
        printf("wrong usage");
        printf("%s host port name\n", argv[0]);
        exit(1);
    }
    int clientfd;
    hostent* host;  /* struct hostent */
    sockaddr_in clientaddr;

    host = gethostbyname(argv[1]);
    DEBUG((clientfd = socket(AF_INET, SOCK_STREAM, 0)) == -1, "fail to create socket");

    bzero(&clientaddr, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons((uint16_t)atoi(argv[2]));
    clientaddr.sin_addr = *((struct in_addr *)host->h_addr);

    DEBUG(connect(clientfd, (struct sockaddr *)&clientaddr, sizeof(struct sockaddr)) == -1, "fail to connect");
    
    char* buf = (char*)malloc(120);
    memset(buf, 0, 120);
    char* buf_read = (char *)malloc(100);

    DEBUG(recv(clientfd, buf, 100, 0) == -1, "fail to recv");
    printf("%s", buf);

    pid_t pid = fork();
    ssize_t sendbytes;
    while (true) {
        if (pid > 0) {
            //父进程发送消息
            strcpy(buf, argv[3]);
            strcat(buf, ":");
            memset(buf_read, 0, 100);
            fgets(buf_read, 100, stdin);
            strcat(buf_read, "  ");
            strncat(buf, buf_read, strlen(buf_read) - 1);
            DEBUG((sendbytes = send(clientfd, buf, strlen(buf), 0)) == -1, "fail to send");
        }
        else if (pid == 0) {
            //子进程接受消息
            memset(buf, 0, 100);
            if (recv(clientfd, buf, 100, 0) <= 0) {
                perror("fail to recv");
                close(clientfd);
                raise(SIGSTOP);
                exit(1);
            }
            printf("\n%s\n", buf);
        }
        else
            perror("fork error");
    }

    free(buf);
    free(buf_read);
    return main_ret;
}
