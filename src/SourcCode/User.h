#ifndef USER_H
#define USER_H
#include <string>

class User {
protected:
    std::string username;
    std::string password;
public:
    User(std::string u, std::string p) : username(u), password(p) {}
    virtual ~User() {}

    std::string getUsername() const { return username; }
    bool checkPassword(const std::string& p) const { return password == p; }
};
#endif