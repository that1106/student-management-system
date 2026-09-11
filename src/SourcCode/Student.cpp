#include "Student.h"

Student::Student(std::string id, std::string name, std::string email, std::string phone, std::string classCode)
    : id(id), name(name), email(email), phone(phone), classCode(classCode) {}

std::string Student::getId() const { return id; }
std::string Student::getName() const { return name; }
std::string Student::getEmail() const { return email; }
std::string Student::getPhone() const { return phone; }
std::string Student::getClassCode() const { return classCode; }

void Student::setId(const std::string& id) { this->id = id; }
void Student::setName(const std::string& name) { this->name = name; }
void Student::setEmail(const std::string& email) { this->email = email; }
void Student::setPhone(const std::string& phone) { this->phone = phone; }
void Student::setClassCode(const std::string& classCode) { this->classCode = classCode; }