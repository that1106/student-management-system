#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "Student.h"
#include <vector>
#include <string>

class StudentController {
private:
    std::vector<Student> students;

public:
    StudentController();

    bool login(const std::string& username, const std::string& password);

    bool addStudent(const std::string& id, const std::string& name, const std::string& email, const std::string& phone, const std::string& classCode, std::string& errorMsg);

    bool updateStudent(const std::string& id, const std::string& name, const std::string& email, const std::string& phone, const std::string& classCode, std::string& errorMsg);

    bool deleteStudent(const std::string& id);

    bool changePassword(const std::string& oldPass, const std::string& newPass, const std::string& confPass, std::string& errorMsg);

    std::vector<Student> getAllStudents();
};

#endif