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
    std::string dateOfBirth; // "YYYY-MM-DD"
    std::string gender;
    std::string address;

public:
    Student(std::string id, std::string name, std::string email, std::string phone,
            std::string classCode, std::string dateOfBirth = "", std::string gender = "",
            std::string address = "");

    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getClassCode() const;
    std::string getDateOfBirth() const;
    std::string getGender() const;
    std::string getAddress() const;

    void setId(const std::string& id);
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPhone(const std::string& phone);
    void setClassCode(const std::string& classCode);
    void setDateOfBirth(const std::string& dateOfBirth);
    void setGender(const std::string& gender);
    void setAddress(const std::string& address);
};

#endif