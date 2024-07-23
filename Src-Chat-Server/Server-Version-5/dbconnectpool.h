#include <mysql/mysql.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <string>
#include <iostream>

class MySQLConn {
public:
    MySQLConn(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db) {
        conn = mysql_init(NULL);
        if (conn == NULL) {
            std::cerr << "mysql_init() failed\n";
            return;
        }

        if (mysql_real_connect(conn, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), 0, NULL, 0) == NULL) {
            std::cerr << "mysql_real_connect() failed\n";
            mysql_close(conn);
            conn = NULL;
        }
    }

    ~MySQLConn() {
        if (conn) {
            mysql_close(conn);
        }
    }

    bool query(const std::string& query) {
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "mysql_query() failed: " << mysql_error(conn) << std::endl;
            return false;
        }
        return true;
    }

    MYSQL_RES* getResult() {
        return mysql_store_result(conn);
    }

    bool update(const std::string& query) {
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "mysql_query() failed: " << mysql_error(conn) << std::endl;
            return false;
        }
        return true;
    }

    MYSQL* getConnection() {
        return conn;
    }

private:
    MYSQL* conn;
};

class ConnPool {
public:
    static std::shared_ptr<ConnPool> getConnPool() {
        static std::shared_ptr<ConnPool> instance(new ConnPool());
        return instance;
    }

    std::shared_ptr<MySQLConn> getConn() {
        std::unique_lock<std::mutex> lock(mtx);

        while (connQueue.empty()) {
            cond.wait(lock);
        }

        auto conn = connQueue.front();
        connQueue.pop();
        return conn;
    }

    void releaseConn(std::shared_ptr<MySQLConn> conn) {
        std::unique_lock<std::mutex> lock(mtx);
        connQueue.push(conn);
        cond.notify_one();
    }

private:
    ConnPool() {
        for (int i = 0; i < poolSize; ++i) {
            connQueue.push(std::make_shared<MySQLConn>("localhost", "yichen", "1234", "chat_user"));
        }
    }

    std::queue<std::shared_ptr<MySQLConn>> connQueue;
    std::mutex mtx;
    std::condition_variable cond;
    const int poolSize = 10;
};
