#ifndef USERS_H
#define USERS_H

#include "User.h"
#include <string>

enum class Role { Admin, Teacher, Student, Unknown };

class Admin : public User {
public:
    std::string fullName;
    std::string email;

    Admin(std::string username, std::string password,
          std::string fullName = "", std::string email = "")
        : User(username, password), fullName(fullName), email(email) {}

    Role role() const { return Role::Admin; }

    std::string welcomeMessage() const override {
        return "Welcome, Admin " + username;
    }
};

class Teacher : public User {
public:
    std::string fullName;
    std::string email;

    Teacher(std::string username, std::string password,
            std::string fullName = "", std::string email = "")
        : User(username, password), fullName(fullName), email(email) {}

    Role role() const { return Role::Teacher; }

    std::string welcomeMessage() const override {
        std::string display = fullName.empty() ? username : fullName;
        return "Welcome, Teacher " + display;
    }
};

class StudentUser : public User {
public:
    std::string studentId;
    std::string fullName;

    StudentUser(std::string username, std::string password,
                std::string studentId = "", std::string fullName = "")
        : User(username, password), studentId(studentId), fullName(fullName) {}

    Role role() const { return Role::Student; }

    std::string welcomeMessage() const override {
        std::string display = fullName.empty() ? username : fullName;
        return "Welcome, Student " + display;
    }
};

#endif