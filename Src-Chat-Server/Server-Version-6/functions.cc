#include "functions.h"
#include "dbconnectpool.h"

static auto pool = ConnPool::getConnPool();


void getUserNameAndPass(const std::string& input, std::string& username, std::string& password) {
    std::istringstream iss(input);
    std::string command;
    iss >> command >> username >> password;
}

bool doesUserExist(const std::string& username) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return false;
    }

    std::string query = "SELECT password FROM Users WHERE username = '" + username + "';";
    bool success = conn->query(query);

    MYSQL_RES* res = success ? conn->getResult() : nullptr;
    bool userExists = res && mysql_fetch_row(res) != nullptr;
    if (res) {
        mysql_free_result(res);
    }

    pool->releaseConn(conn);
    return userExists;
}

bool verifyPassword(const std::string& username, const std::string& inputPassword) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return false;
    }

    std::string query = "SELECT password FROM Users WHERE username = '" + username + "';";
    bool success = conn->query(query);

    MYSQL_RES* res = success ? conn->getResult() : nullptr;
    MYSQL_ROW row = res ? mysql_fetch_row(res) : nullptr;
    bool passwordMatches = row && (inputPassword == row[0]);
    if (res) {
        mysql_free_result(res);
    }

    pool->releaseConn(conn);
    return passwordMatches;
}

bool insertNewUser(const std::string& username, const std::string& password) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return false;
    }

    std::string query = "INSERT INTO Users (`username`, `password`) VALUES ('" + username + "', '" + password + "');";
    if (!conn->update(query)) {
        std::cerr << "Insert failed" << std::endl;
        pool->releaseConn(conn);
        return false;
    }

    query = "CREATE TABLE " + username + "_friends (friends CHAR(30));";
    if (!conn->update(query)) {
        std::cerr << "Create table failed" << std::endl;
        pool->releaseConn(conn);
        return false;
    }

    query = "INSERT INTO " + username + "_friends VALUES ('" + username + "');";
    if (!conn->update(query)) {
        std::cerr << "Insert into friends table failed" << std::endl;
        pool->releaseConn(conn);
        return false;
    }

    pool->releaseConn(conn);
    return true;
}

void get_src_tar_user_msg(const std::string& input, std::string& src_username, std::string& tar_username, std::string& msg) {
    std::istringstream iss(input);
    iss >> src_username >> tar_username >> msg;
}

void parse_add_friend_cmd(const std::string& cmd, std::string& request_user, std::string& target_user) {
    std::istringstream iss(cmd);
    iss >> request_user >> target_user;
}

std::string produce_add_friend_cmd(const std::string& req_user, const std::string& tar_user) {
    return "9 " + req_user + " " + tar_user;
}

bool add_friend(const std::string& req_user, const std::string& tar_user) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return false;
    }

    std::string query = "INSERT INTO " + req_user + "_friends VALUES ('" + tar_user + "');";
    if (!conn->update(query)) {
        std::cerr << "Insert into friends table failed" << std::endl;
        pool->releaseConn(conn);
        return false;
    }

    pool->releaseConn(conn);
    return true;
}

std::string produce_friend_verification_msg(const std::string& username) {
    return username + " has added you as a friend";
}

bool checkUserExists(const std::string& username) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return false;
    }

    std::string query = "SELECT username FROM Users WHERE username = '" + username + "';";
    bool success = conn->query(query);

    MYSQL_RES* res = success ? conn->getResult() : nullptr;
    bool userExists = res && mysql_fetch_row(res) != nullptr;
    if (res) {
        mysql_free_result(res);
    }

    pool->releaseConn(conn);
    return userExists;
}

bool checkFrinedExists(const std::string& srcname, const std::string& tarname) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return false;
    }

    std::string query = "SELECT friends FROM " + srcname + "_friends WHERE friends = '" + tarname + "';";
    bool success = conn->query(query);

    MYSQL_RES* res = success ? conn->getResult() : nullptr;
    bool friendExists = res && mysql_fetch_row(res) != nullptr;
    if (res) {
        mysql_free_result(res);
    }

    pool->releaseConn(conn);
    return friendExists;
}

bool tableExists(const std::string& prefix) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return false;
    }

    std::string tableName = prefix + "_friends";
    std::string query = "SHOW TABLES LIKE '" + tableName + "';";
    bool success = conn->query(query);

    MYSQL_RES* res = success ? conn->getResult() : nullptr;
    bool exists = res && mysql_fetch_row(res) != nullptr;
    if (res) {
        mysql_free_result(res);
    }

    pool->releaseConn(conn);
    return exists;
}

std::string fetchTableData(const std::string& prefix) {
    auto conn = pool->getConn();
    if (!conn) {
        std::cerr << "Failed to get database connection from pool" << std::endl;
        return "";
    }

    std::string tableName = prefix + "_friends";
    std::stringstream resultStream;

    std::string query = "SELECT * FROM " + tableName;
    if (!conn->query(query)) {
        return "";
    }

    MYSQL_RES* res = conn->getResult();
    if (!res) {
        return "";
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL) {
        for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
            resultStream << (row[i] ? row[i] : "NULL") << " ";
        }
    }

    mysql_free_result(res);
    pool->releaseConn(conn);
    return resultStream.str();
}


void splitInput(const std::string& input, std::string& num, std::string& user1, std::string& user2, std::string& message) {
    std::istringstream iss(input);
    std::string temp;

    // 读取第一个部分
    if (iss >> num) {
        // 读取第一个用户
        if (iss >> user1) {
            // 读取第二个用户
            if (iss >> user2) {
                // 读取消息，直到行末
                std::getline(iss, message);
                // 消息开头可能有一个空格，所以我们需要去除它
                if (!message.empty() && message[0] == ' ') {
                    message.erase(0, 1);
                }
            }
        }
    }
}


int listenSocketInit(const char* ip, unsigned short port) {
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sockfd == -1) 
        return -1;

    /* 端口地址复用 */
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof(on));

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);
 
    if (bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) == -1)
        return -1;

    if (listen(sockfd, 20) == -1)
        return -1;

    return sockfd;
}

int epollInit(int socketfd) {
    int epollfd = epoll_create(1);
    if (epollfd == -1)
        return -1;

    struct epoll_event e;
    memset(&e, 0, sizeof(e));
    e.events = EPOLLIN | EPOLLRDHUP;
    e.data.fd = socketfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &e) == -1)
        return -1;
    
    return epollfd;
}

bool setSocketNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "fcntl(F_GETFL) failed: " << strerror(errno) << std::endl;
        return false;
    }

    // 设置非阻塞标志
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "fcntl(F_SETFL) failed: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool addSocketToEpoll(int epollfd, int sockfd, uint32_t events) {
    struct epoll_event event;
    event.events = events;
    event.data.fd = sockfd;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
        std::cerr << "epoll_ctl(EPOLL_CTL_ADD) failed: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool removeSocketFromEpoll(int epollfd, int sockfd) {
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, nullptr) == -1) {
        std::cerr << "epoll_ctl(EPOLL_CTL_DEL) failed: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

std::string getCurrentTimestamp() {
    using namespace std::chrono;
    
    // 获取当前时间点
    auto now = system_clock::now();
    
    // 转换为时间_t 类型
    auto now_c = system_clock::to_time_t(now);
    
    // 将时间_t 转换为 tm 结构
    std::tm* now_tm = std::localtime(&now_c);
    
    // 创建一个字符串流
    std::ostringstream oss;
    
    // 格式化时间为字符串
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    
    return oss.str();
}

bool createGroup(const std::string &groupName, const std::string &username) {
    auto connPool = ConnPool::getConnPool();
    auto conn = connPool->getConn();

    std::string query = "INSERT INTO chatRoomGroups (group_name) VALUES ('" + groupName + "');";
    if (!conn->query(query)) {
        return false;
    }

    query = "CREATE TABLE `" + groupName + "_members` ("
            "`id` INT AUTO_INCREMENT PRIMARY KEY, "
            "`username` VARCHAR(50) NOT NULL, "
            "`type` ENUM('master', 'manager', 'member') NOT NULL) "
            "ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";
    if (!conn->update(query)) {
        return false;
    }

    query = "INSERT INTO `" + groupName + "_members` (username, type) VALUES ('" + username + "', 'master');";
    if (!conn->update(query)) {
        return false;
    }

    connPool->releaseConn(conn);
    return true;
}

bool groupExists(const std::string &groupName) {
    auto connPool = ConnPool::getConnPool();
    auto conn = connPool->getConn();

    std::string query = "SELECT 1 FROM chatRoomGroups WHERE group_name = '" + groupName + "';";
    if (!conn->query(query)) {
        connPool->releaseConn(conn);
        return false;
    }

    MYSQL_RES *res = conn->getResult();
    if (res == NULL) {
        connPool->releaseConn(conn);
        return false;
    }

    bool exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);
    connPool->releaseConn(conn);
    return exists;
}

bool userInGroup(const std::string &username, const std::string &groupName) {
    auto connPool = ConnPool::getConnPool();
    auto conn = connPool->getConn();

    std::string query = "SELECT 1 FROM `" + groupName + "_members` WHERE username = '" + username + "';";
    if (!conn->query(query)) {
        connPool->releaseConn(conn);
        return false;
    }

    MYSQL_RES *res = conn->getResult();
    if (res == NULL) {
        connPool->releaseConn(conn);
        return false;
    }

    bool exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);
    connPool->releaseConn(conn);
    return exists;
}

bool addUserToGroup(const std::string &username, const std::string &groupName, bool isManager) {
    auto connPool = ConnPool::getConnPool();
    auto conn = connPool->getConn();

    std::string type = isManager ? "manager" : "member";
    std::string query = "INSERT INTO `" + groupName + "_members` (username, type) VALUES ('" + username + "', '" + type + "');";
    if (!conn->update(query)) {
        connPool->releaseConn(conn);
        return false;
    }

    connPool->releaseConn(conn);
    return true;
}


std::string getUsersInGroup(const std::string &groupName) {
    std::string usernames;
    auto connPool = ConnPool::getConnPool();
    auto conn = connPool->getConn();

    // 构建查询语句
    std::string query = "SELECT username FROM `" + groupName + "_members`;";
    if (!conn->query(query)) {
        connPool->releaseConn(conn);
        return usernames;  // 返回空的用户列表
    }

    MYSQL_RES *res = conn->getResult();
    if (res == NULL) {
        connPool->releaseConn(conn);
        return usernames;  // 返回空的用户列表
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL) {
        if (row[0] != NULL) {
            usernames += row[0];
            usernames += " ";
        }
    }

    mysql_free_result(res);
    connPool->releaseConn(conn);
    return usernames;
}





bool addGroupToUserJoinedGroups(const std::string &username, const std::string &groupName) {
    auto conn = pool->getConn();

    std::string tableName = username + "_joined_groups";
    std::string query = "INSERT INTO `" + tableName + "` (groupname) VALUES ('" + groupName + "');";

    bool result = conn->query(query);
    pool->releaseConn(conn);
    return result;
}

std::string getJoinedGroups(const std::string &username) {
    auto connPool = ConnPool::getConnPool();
    auto conn = connPool->getConn();

    std::string tableName = username + "_joined_groups";
    std::string query = "SELECT groupname FROM " + tableName + ";";

    if (!conn->query(query)) {
        connPool->releaseConn(conn);
        return "";
    }

    MYSQL_RES *res = conn->getResult();
    if (res == NULL) {
        connPool->releaseConn(conn);
        return "";
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    mysql_free_result(res);

    if (row == NULL) {
        connPool->releaseConn(conn);
        return "";
    }

    query = "SELECT groupname FROM `" + tableName + "`;";
    if (!conn->query(query)) {
        connPool->releaseConn(conn);
        return "";
    }

    res = conn->getResult();
    if (res == NULL) {
        connPool->releaseConn(conn);
        return "";
    }

    std::vector<std::string> groups;
    while ((row = mysql_fetch_row(res)) != NULL) {
        if (row[0] != NULL) {
            groups.push_back(row[0]);
        }
    }

    mysql_free_result(res);
    connPool->releaseConn(conn);

    std::stringstream ss;
    for (const auto &group : groups) {
        if (!ss.str().empty()) {
            ss << " ";
        }
        ss << group;
    }

    return ss.str();
}


void groupGetCreateUserNameAndGroupName(const std::string& input, std::string& createUserName, std::string& GroupName) {
    std::istringstream iss(input);
    iss >> createUserName >> GroupName;
}

bool doesJoinedGroupsTableExist(const std::string &username) {
    auto conn = pool->getConn();

    std::string tableName = username + "_joined_groups";
    std::string query = "SELECT COUNT(*) FROM " + tableName + ";";

    if (!conn->query(query)) {
        pool->releaseConn(conn);
        return false;
    }

    MYSQL_RES *res = conn->getResult();
    if (res == NULL) {
        pool->releaseConn(conn);
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    bool exists = row && atoi(row[0]) > 0;

    mysql_free_result(res);
    pool->releaseConn(conn);

    return exists;
}

void get_src_tar_group_msg(const std::string& input, std::string& master, std::string& tar, std::string& group, std::string& msg) {
    std::istringstream iss(input);
    iss >> master >> tar >> group >> msg;
}

std::string getGroupMasterUser(const std::string &groupName) {
    auto conn = pool->getConn();

    std::string tableName = groupName + "_members";
    std::string query = "SELECT username FROM `" + tableName + "` WHERE id = 1;";

    if (!conn->query(query)) {
        pool->releaseConn(conn);
        return "";
    }

    MYSQL_RES *res = conn->getResult();
    if (res == NULL) {
        pool->releaseConn(conn);
        return "";
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    std::string masterUsername;
    if (row && row[0]) {
        masterUsername = row[0];
    }

    mysql_free_result(res);
    pool->releaseConn(conn);

    return masterUsername;
}

void processMessage(const std::string &rawData, std::string& username,std::string& groupName,std::string& message) {
    std::stringstream ss(rawData);
    std::string prefix;

    ss >> prefix >> username >> groupName;
    std::getline(ss, message);
    message = message.substr(1); // 去掉开头的空格

    if (prefix != "3") {
        return;
    }

    auto conn = pool->getConn();

    // 检查用户是否存在
    std::string userQuery = "SELECT COUNT(*) FROM Users WHERE username = '" + username + "';";
    if (!conn->query(userQuery)) {
        pool->releaseConn(conn);
        return;
    }

    MYSQL_RES *res = conn->getResult();
    if (res == NULL) {
        pool->releaseConn(conn);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row || atoi(row[0]) == 0) {
        mysql_free_result(res);
        pool->releaseConn(conn);
        return;
    }
    mysql_free_result(res);

    // 检查群聊是否存在
    std::string groupQuery = "SELECT COUNT(*) FROM chatRoomGroups WHERE group_name = '" + groupName + "';";
    if (!conn->query(groupQuery)) {
        pool->releaseConn(conn);
        return;
    }

    res = conn->getResult();
    if (res == NULL) {
        pool->releaseConn(conn);
        return;
    }

    row = mysql_fetch_row(res);
    if (!row || atoi(row[0]) == 0) {
        mysql_free_result(res);
        pool->releaseConn(conn);
        return;
    }
    mysql_free_result(res);

    pool->releaseConn(conn);
}

bool createJoinedGroupsTable(const std::string& username) {
    auto conn = pool->getConn();

    std::string tableName = username + "_joined_groups";
    std::string query = "CREATE TABLE IF NOT EXISTS `" + tableName + "` ("
                        "id INT NOT NULL AUTO_INCREMENT, "
                        "groupname VARCHAR(100) DEFAULT NULL, "
                        "PRIMARY KEY (id)) "
                        "ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;";

    bool result = conn->query(query);
    pool->releaseConn(conn);

    return result;
}
