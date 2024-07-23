#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>  /* htonl()、htons() */
#include <sys/epoll.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>


#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>


#include <mysql/mysql.h>

void getUserNameAndPass(const std::string& input, std::string& username, std::string& password);
bool doesUserExist(const std::string& username);
bool verifyPassword(const std::string& username, const std::string& inputPassword);
bool insertNewUser(const std::string& username, const std::string& password);
void get_src_tar_user_msg(const std::string& input, std::string& src_username, std::string& tar_username, std::string& msg);
void parse_add_friend_cmd(const std::string& cmd, std::string& request_user, std::string& target_user);
std::string produce_add_friend_cmd(const std::string& req_user, const std::string& tar_user);
bool add_friend(const std::string& req_user, const std::string& tar_user);
std::string produce_friend_verification_msg(const std::string& username);
bool checkUserExists(const std::string& username);
bool checkFrinedExists(const std::string& srcname, const std::string& tarname);
bool tableExists(const std::string& prefix);
std::string fetchTableData(const std::string& prefix);
int listenSocketInit(const char* ip, unsigned short port);
int epollInit(int sockfd);
bool setSocketNonBlocking(int sockfd);
bool addSocketToEpoll(int epollfd, int sockfd, uint32_t events = EPOLLIN | EPOLLRDHUP | EPOLLET);
bool removeSocketFromEpoll(int epollfd, int sockfd);
std::string getCurrentTimestamp(); 
void splitInput(const std::string& input, std::string& num, std::string& user1, std::string& user2, std::string& message);

#endif