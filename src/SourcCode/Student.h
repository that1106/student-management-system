#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    std::string classCode;

public:
    Student(std::string id, std::string name, std::string email, std::string phone, std::string classCode);

    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getClassCode() const;

    void setId(const std::string& id);
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPhone(const std::string& phone);
    void setClassCode(const std::string& classCode);
};

#endif