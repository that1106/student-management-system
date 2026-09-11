#ifndef STUDENT_H
#define STUDENT_H
#include "User.h"
#include <string>

class Student : public User {
private:
    std::string studentId;
    std::string fullName;
    std::string phone;
    std::string classCode;
public:
    Student(std::string user, std::string pass, std::string id, std::string name, std::string ph, std::string cls)
        : User(user, pass), studentId(id), fullName(name), phone(ph), classCode(cls) {}

    std::string getId() const { return studentId; }
    std::string getName() const { return fullName; }
    std::string getPhone() const { return phone; }
    std::string getClassCode() const { return classCode; }
};
#endif