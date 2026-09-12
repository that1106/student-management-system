#ifndef STUDENTCONTROLLER_H
#define STUDENTCONTROLLER_H

#include "Student.h"
#include <vector>
#include <string>
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

    std::vector<GradeRow> getTeacherGradebook(const std::string& teacherUsername);
    bool updateGrade(int enrollmentId, double score, std::string& errorMsg);
    bool assignTeacher(const std::string& teacherName, const std::string& classCode,
                       const std::string& subjectCode, std::string& errorMsg);


    StudentProfile getStudentProfile(const std::string& username);
    std::vector<SubjectResult> getStudentResults(const std::string& username);
    static double computeGPA(const std::vector<SubjectResult>& results);
};

#endif // STUDENTCONTROLLER_H