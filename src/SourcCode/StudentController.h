#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "Student.h"
#include "User.h"
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QString>

struct GradeRow {
    std::string studentId;
    std::string studentName;
    std::string subjectCode;
    std::string subjectName;
    int enrollmentId;
    double score;
};

struct SubjectResult {
    std::string subjectName;
    int credits;
    double score;
    int enrollmentId;
};

struct StudentProfile {
    bool found = false;
    std::string id, name, email, phone, classCode, dateOfBirth, gender, address;
};

struct ClassInfo {
    std::string code;
    std::string name;
};

struct SubjectInfo {
    std::string code;
    std::string name;
    int credits;
};

struct EnrollmentInfo {
    int id;
    std::string studentId;
    std::string studentName;
    std::string subjectCode;
    std::string subjectName;
    double score;
};

struct AssignmentInfo {
    int id;
    std::string teacherName;
    std::string classCode;
    std::string subjectCode;
};

class StudentController {
private:
    QSqlDatabase db;
    std::string currentUsername;
    int currentRole = 0;

    static QString hashPassword(const std::string& plain);
    void seedDemoData();
    bool classExists(const std::string& classCode);

public:
    StudentController();
    ~StudentController();

    int login(const std::string& username, const std::string& password);
    void logout();
    std::string getCurrentUsername() const;
    int getCurrentRole() const;
    bool changePassword(const std::string& username, const std::string& oldPass,
                        const std::string& newPass, const std::string& confPass,
                        std::string& errorMsg);

    bool addStudent(const std::string& id, const std::string& name, const std::string& email,
                    const std::string& phone, const std::string& classCode,
                    const std::string& dateOfBirth, const std::string& gender,
                    const std::string& address, std::string& errorMsg);
    bool updateStudent(const std::string& id, const std::string& name, const std::string& email,
                       const std::string& phone, const std::string& classCode,
                       const std::string& dateOfBirth, const std::string& gender,
                       const std::string& address, std::string& errorMsg);
    bool deleteStudent(const std::string& id);
    std::vector<Student> getAllStudents();
    std::vector<Student> searchStudents(const std::string& keyword);

    bool addClass(const std::string& classCode, const std::string& className, std::string& errorMsg);
    bool addSubject(const std::string& subjectCode, const std::string& subjectName, int credits, std::string& errorMsg);
    bool enrollStudent(const std::string& studentId, const std::string& subjectCode, std::string& errorMsg);
    bool dropEnrollment(int enrollmentId, std::string& errorMsg);
    std::vector<ClassInfo> getAllClasses();
    std::vector<SubjectInfo> getAllSubjects();
    std::vector<SubjectInfo> getAvailableSubjects(const std::string& studentId);
    std::vector<EnrollmentInfo> getAllEnrollments();

    std::vector<GradeRow> getTeacherGradebook(const std::string& teacherUsername);
    bool updateGrade(int enrollmentId, double score, std::string& errorMsg);
    bool assignTeacher(const std::string& teacherName, const std::string& classCode,
                       const std::string& subjectCode, std::string& errorMsg);
    std::vector<AssignmentInfo> getAllAssignments();

    bool addAccount(const std::string& username, const std::string& password,
                    const std::string& role, std::string& errorMsg);
    std::vector<std::pair<std::string, std::string>> getAllAccounts();

    StudentProfile getStudentProfile(const std::string& username);
    std::vector<SubjectResult> getStudentResults(const std::string& username);
    static double computeGPA(const std::vector<SubjectResult>& results);

    std::unique_ptr<User> buildSessionUser(int role, const std::string& username);
};

#endif