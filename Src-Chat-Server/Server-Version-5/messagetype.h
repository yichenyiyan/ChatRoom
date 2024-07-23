#ifndef MESSAGETYPE_H_
#define MESSAGETYPE_H_

#include <string>

class Message {
public:
    std::string msg;
    Message(std::string message) : msg(message) {}
    virtual ~Message() {}
    virtual const std::string& getMsg() { return msg; }
};

class PrivateMsg final : public Message {
public:
    explicit PrivateMsg(std::string src, std::string tar, std::string _msg) :src_user(src), target_user(tar), Message(_msg) {}
    ~PrivateMsg() {}
    inline const std::string& getSrcUser() { return src_user; }  
    inline const std::string& getTarUser() { return target_user; }
private:
    std::string src_user;
    std::string target_user;
};

class GroupMsg final : public Message {
public:
    explicit GroupMsg(std::string src, int id, std::string _msg) :src_user(src), group_id(id), Message(_msg) {}
    ~GroupMsg() {}
    inline const std::string& getSrcUser() { return src_user; }  
    inline const int& getGroupId() { return group_id; }
private:
    int group_id; 
    std::string src_user;
    
};

#endif