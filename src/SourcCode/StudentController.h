#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H
#include "Student.h"
#include <vector>
#include <string>
#include <cctype>

class StudentController {
private:
    std::vector<Student> students;
    std::string adminUser = "admin";
    std::string adminPass = "Admin@123";
public:
    StudentController() {
        students.push_back(Student("student1", "123", "ST000001", "Tran Van Bao Duy", "0123456789", "IT01"));
    }

    bool login(std::string user, std::string pass) {
        return (user == adminUser && pass == adminPass);
    }

    bool changePassword(std::string oldP, std::string newP, std::string confP, std::string& error) {
        if (oldP != adminPass) {
            error = "Mật khẩu hiện tại không đúng!"; return false;
        }
        if (newP != confP) {
            error = "Mật khẩu xác nhận không khớp!"; return false;
        }
        if (newP.length() < 8) {
            error = "Mật khẩu mới phải có ít nhất 8 ký tự, 1 chữ hoa, 1 chữ thường và 1 số!"; return false;
        }
        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (char c : newP) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
        }
        if (!hasUpper || !hasLower || !hasDigit) {
            error = "Mật khẩu mới phải có ít nhất 8 ký tự, 1 chữ hoa, 1 chữ thường và 1 số!"; return false;
        }
        adminPass = newP;
        return true;
    }

    bool addStudent(std::string id, std::string name, std::string phone, std::string cls, std::string& error) {
        if (id.length() != 8 || id.substr(0, 2) != "ST") {
            error = "Mã sinh viên phải có định dạng ST + 6 chữ số (VD: ST000001)!"; return false;
        }
        if (name.empty()) { error = "Họ tên không được để trống!"; return false; }
        for (char c : name) {
            if (isdigit(c)) { error = "Full name must not contain numeric characters!"; return false; }
        }
        if (phone.length() != 10) { error = "Số điện thoại phải có đúng 10 chữ số!"; return false; }
        for(const auto& s : students) {
            if(s.getId() == id) { error = "Mã sinh viên đã tồn tại!"; return false; }
        }
        students.push_back(Student(id, id, id, name, phone, cls));
        return true;
    }

    bool updateStudent(std::string id, std::string phone, std::string cls, std::string& error) {
        if (phone.length() != 10) { error = "Số điện thoại phải có đúng 10 chữ số!"; return false; }
        for(auto& s : students) {
            if(s.getId() == id) {
                std::string name = s.getName();
                s = Student(id, id, id, name, phone, cls);
                return true;
            }
        }
        error = "Không tìm thấy sinh viên!"; return false;
    }

    bool deleteStudent(std::string id) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getId() == id) {
                students.erase(it); return true;
            }
        }
        return false;
    }

    std::vector<Student> getAllStudents() { return students; }
};
#endif