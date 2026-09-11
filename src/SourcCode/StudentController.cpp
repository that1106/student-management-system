#include "StudentController.h"
#include <regex>
#include <algorithm>

StudentController::StudentController() {
    students.push_back(Student("ST000001", "Nguyen Van A", "nva@school.edu.vn", "0901234567", "SE101"));
}

bool StudentController::login(const std::string& username, const std::string& password) {
    return (username == "admin" && password == "Admin@123");
}

bool StudentController::addStudent(const std::string& id, const std::string& name, const std::string& email, const std::string& phone, const std::string& classCode, std::string& errorMsg) {
    // 1. Kiểm tra rỗng
    if (id.empty() || name.empty() || email.empty() || phone.empty() || classCode.empty()) {
        errorMsg = "Vui lòng nhập đầy đủ thông tin!";
        return false;
    }

    std::regex idRegex("^ST\\d{6}$");
    if (!std::regex_match(id, idRegex)) {
        errorMsg = "Mã SV không hợp lệ! Định dạng chuẩn: ST + 6 chữ số (VD: ST000001).";
        return false;
    }

    for (const auto& s : students) {
        if (s.getId() == id) {
            errorMsg = "Mã sinh viên đã tồn tại!";
            return false;
        }
    }

    if (std::any_of(name.begin(), name.end(), ::isdigit)) {
        errorMsg = "Họ và tên không được chứa chữ số!";
        return false;
    }

    std::regex phoneRegex("^\\d{10}$");
    if (!std::regex_match(phone, phoneRegex)) {
        errorMsg = "Số điện thoại phải bao gồm đúng 10 chữ số!";
        return false;
    }

    std::regex emailRegex("^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");
    if (!std::regex_match(email, emailRegex)) {
        errorMsg = "Định dạng Email không hợp lệ!";
        return false;
    }

    students.push_back(Student(id, name, email, phone, classCode));
    return true;
}

bool StudentController::updateStudent(const std::string& id, const std::string& name, const std::string& email, const std::string& phone, const std::string& classCode, std::string& errorMsg) {
    if (name.empty() || email.empty() || phone.empty() || classCode.empty()) {
        errorMsg = "Vui lòng nhập đầy đủ thông tin!"; return false;
    }

    if (std::any_of(name.begin(), name.end(), ::isdigit)) {
        errorMsg = "Họ và tên không được chứa chữ số!"; return false;
    }

    std::regex phoneRegex("^\\d{10}$");
    if (!std::regex_match(phone, phoneRegex)) {
        errorMsg = "Số điện thoại phải bao gồm đúng 10 chữ số!"; return false;
    }

    std::regex emailRegex("^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");
    if (!std::regex_match(email, emailRegex)) {
        errorMsg = "Định dạng Email không hợp lệ!"; return false;
    }

    for (auto& s : students) {
        if (s.getId() == id) {
            s.setName(name);
            s.setEmail(email);
            s.setPhone(phone);
            s.setClassCode(classCode);
            return true;
        }
    }
    errorMsg = "Không tìm thấy sinh viên để cập nhật!";
    return false;
}

bool StudentController::deleteStudent(const std::string& id) {
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getId() == id) {
            students.erase(it);
            return true;
        }
    }
    return false;
}

bool StudentController::changePassword(const std::string& oldPass, const std::string& newPass, const std::string& confPass, std::string& errorMsg) {
    if (oldPass != "Admin@123") { errorMsg = "Mật khẩu hiện tại không đúng!"; return false; }
    if (newPass != confPass) { errorMsg = "Mật khẩu xác nhận không khớp!"; return false; }
    if (newPass.length() < 6) { errorMsg = "Mật khẩu phải có ít nhất 6 ký tự!"; return false; }
    return true;
}

std::vector<Student> StudentController::getAllStudents() {
    return students;
}