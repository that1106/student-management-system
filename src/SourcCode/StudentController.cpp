#include "StudentController.h"
#include "Users.h"
#include <regex>
#include <algorithm>
#include <cctype>
#include <QDebug>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDate>

StudentController::StudentController() {
    if (QSqlDatabase::isDriverAvailable("QMYSQL")) {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("127.0.0.1");
        db.setPort(3306);
        db.setDatabaseName("student_management");
        db.setUserName("root");
        db.setPassword("");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("student_management.db");
    }

    if (!db.open()) {
        qWarning() << "Could not open database:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS accounts ("
               "username VARCHAR(50) PRIMARY KEY, password VARCHAR(255), role VARCHAR(20))");
    query.exec("CREATE TABLE IF NOT EXISTS students ("
               "id VARCHAR(20) PRIMARY KEY, name VARCHAR(100), email VARCHAR(100), "
               "phone VARCHAR(20), classCode VARCHAR(20), dateOfBirth VARCHAR(20), "
               "gender VARCHAR(10), address VARCHAR(255))");
    query.exec("CREATE TABLE IF NOT EXISTS classes ("
               "classCode VARCHAR(20) PRIMARY KEY, className VARCHAR(100))");
    query.exec("CREATE TABLE IF NOT EXISTS subjects ("
               "subjectCode VARCHAR(20) PRIMARY KEY, subjectName VARCHAR(100), credits INTEGER)");
    query.exec("CREATE TABLE IF NOT EXISTS enrollments ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, studentId VARCHAR(20), "
               "subjectCode VARCHAR(20), score REAL DEFAULT -1.0)");
    query.exec("CREATE TABLE IF NOT EXISTS assignments ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, teacherName VARCHAR(100), "
               "classCode VARCHAR(20), subjectCode VARCHAR(20))");
    query.exec("CREATE TABLE IF NOT EXISTS teachers ("
               "username VARCHAR(50) PRIMARY KEY, fullName VARCHAR(100), email VARCHAR(100))");

    query.exec("ALTER TABLE students ADD COLUMN dateOfBirth VARCHAR(20)");
    query.exec("ALTER TABLE students ADD COLUMN gender VARCHAR(10)");
    query.exec("ALTER TABLE students ADD COLUMN address VARCHAR(255)");

    bool isSqliteIdx = (db.driverName() == "QSQLITE");
    if (isSqliteIdx) {
        query.exec("CREATE INDEX IF NOT EXISTS idx_students_name ON students(name)");
        query.exec("CREATE INDEX IF NOT EXISTS idx_students_email ON students(email)");
        query.exec("CREATE INDEX IF NOT EXISTS idx_students_phone ON students(phone)");
        query.exec("CREATE INDEX IF NOT EXISTS idx_students_class ON students(classCode)");
    } else {
        query.exec("CREATE INDEX idx_students_name ON students(name)");
        query.exec("CREATE INDEX idx_students_email ON students(email)");
        query.exec("CREATE INDEX idx_students_phone ON students(phone)");
        query.exec("CREATE INDEX idx_students_class ON students(classCode)");
    }

    seedDemoData();
}

StudentController::~StudentController() {
    if (db.isOpen()) { db.close(); }
}

QString StudentController::hashPassword(const std::string& plain) {
    return QString(QCryptographicHash::hash(QByteArray::fromStdString(plain),
                                            QCryptographicHash::Sha256).toHex());
}

void StudentController::seedDemoData() {
    bool isSqlite = (db.driverName() == "QSQLITE");

    auto seedAccount = [&](const std::string& user, const std::string& pass, const std::string& role) {
        QSqlQuery check;
        check.prepare("SELECT username FROM accounts WHERE username = :u");
        check.bindValue(":u", QString::fromStdString(user));
        check.exec();
        if (check.next()) return;

        QSqlQuery ins;
        ins.prepare("INSERT INTO accounts (username, password, role) VALUES (:u, :p, :r)");
        ins.bindValue(":u", QString::fromStdString(user));
        ins.bindValue(":p", hashPassword(pass));
        ins.bindValue(":r", QString::fromStdString(role));
        ins.exec();
    };

    seedAccount("admin", "Admin@123", "admin");
    seedAccount("teacher01", "Teacher@123", "teacher");
    seedAccount("ST000002", "Student@123", "student");

    QSqlQuery q;

    if (isSqlite) q.exec("INSERT OR IGNORE INTO teachers (username, fullName, email) VALUES "
               "('teacher01', 'Nguyen Van A', 'teacher01@gmail.com')");
    else q.exec("INSERT IGNORE INTO teachers (username, fullName, email) VALUES "
               "('teacher01', 'Nguyen Van A', 'teacher01@gmail.com')");

    if (isSqlite) q.exec("INSERT OR IGNORE INTO classes (classCode, className) VALUES "
               "('ST101', 'Software Engineering 101')");
    else q.exec("INSERT IGNORE INTO classes (classCode, className) VALUES "
               "('ST101', 'Software Engineering 101')");

    if (isSqlite) {
        q.exec("INSERT OR IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('MATH101', 'Advanced Mathematics', 3)");
        q.exec("INSERT OR IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('PRO101', 'Object Oriented Programming', 4)");
    } else {
        q.exec("INSERT IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('MATH101', 'Advanced Mathematics', 3)");
        q.exec("INSERT IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('PRO101', 'Object Oriented Programming', 4)");
    }

    QSqlQuery checkStu;
    checkStu.prepare("SELECT id FROM students WHERE id = :id");
    checkStu.bindValue(":id", "ST000002");
    checkStu.exec();
    if (!checkStu.next()) {
        QSqlQuery ins;
        ins.prepare("INSERT INTO students (id, name, email, phone, classCode, dateOfBirth, gender, address) "
                    "VALUES (:id, :name, :email, :phone, :class, :dob, :gender, :address)");
        ins.bindValue(":id", "ST000002");
        ins.bindValue(":name", "Tran Van Bao Duy");
        ins.bindValue(":email", "student@gmail.com");
        ins.bindValue(":phone", "0900000002");
        ins.bindValue(":class", "ST101");
        ins.bindValue(":dob", "2004-05-10");
        ins.bindValue(":gender", "Male");
        ins.bindValue(":address", "Ho Chi Minh City");
        ins.exec();
    }

    auto seedEnrollment = [&](const std::string& stu, const std::string& sub, double score) {
        QSqlQuery check;
        check.prepare("SELECT id FROM enrollments WHERE studentId = :s AND subjectCode = :c");
        check.bindValue(":s", QString::fromStdString(stu));
        check.bindValue(":c", QString::fromStdString(sub));
        check.exec();
        if (check.next()) return;

        QSqlQuery ins;
        ins.prepare("INSERT INTO enrollments (studentId, subjectCode, score) VALUES (:s, :c, :sc)");
        ins.bindValue(":s", QString::fromStdString(stu));
        ins.bindValue(":c", QString::fromStdString(sub));
        ins.bindValue(":sc", score);
        ins.exec();
    };
    seedEnrollment("ST000002", "MATH101", 8.5);
    seedEnrollment("ST000002", "PRO101", 9.0);

    auto seedAssignment = [&](const std::string& teacher, const std::string& cls, const std::string& sub) {
        QSqlQuery check;
        check.prepare("SELECT id FROM assignments WHERE teacherName = :t AND classCode = :c AND subjectCode = :s");
        check.bindValue(":t", QString::fromStdString(teacher));
        check.bindValue(":c", QString::fromStdString(cls));
        check.bindValue(":s", QString::fromStdString(sub));
        check.exec();
        if (check.next()) return;

        QSqlQuery ins;
        ins.prepare("INSERT INTO assignments (teacherName, classCode, subjectCode) VALUES (:t, :c, :s)");
        ins.bindValue(":t", QString::fromStdString(teacher));
        ins.bindValue(":c", QString::fromStdString(cls));
        ins.bindValue(":s", QString::fromStdString(sub));
        ins.exec();
    };
    seedAssignment("teacher01", "ST101", "MATH101");
    seedAssignment("teacher01", "ST101", "PRO101");
}

bool StudentController::classExists(const std::string& classCode) {
    QSqlQuery q;
    q.prepare("SELECT classCode FROM classes WHERE classCode = :c");
    q.bindValue(":c", QString::fromStdString(classCode));
    q.exec();
    return q.next();
}

int StudentController::login(const std::string& username, const std::string& password) {
    QSqlQuery query;
    query.prepare("SELECT role, password FROM accounts WHERE username = :user");
    query.bindValue(":user", QString::fromStdString(username));
    query.exec();

    if (!query.next()) return 0;

    QString stored = query.value("password").toString();
    QString inputHash = hashPassword(password);

    bool matches = (stored == inputHash);

    if (!matches && stored == QString::fromStdString(password)) {
        matches = true;
        QSqlQuery upgrade;
        upgrade.prepare("UPDATE accounts SET password = :p WHERE username = :u");
        upgrade.bindValue(":p", inputHash);
        upgrade.bindValue(":u", QString::fromStdString(username));
        upgrade.exec();
    }

    if (!matches) return 0;

    std::string role = query.value("role").toString().toStdString();
    currentUsername = username;

    if (role == "admin")   { currentRole = 1; return 1; }
    if (role == "teacher") { currentRole = 2; return 2; }
    if (role == "student") { currentRole = 3; return 3; }
    return 0;
}

void StudentController::logout() {
    currentUsername.clear();
    currentRole = 0;
}

std::string StudentController::getCurrentUsername() const { return currentUsername; }
int StudentController::getCurrentRole() const { return currentRole; }

bool StudentController::changePassword(const std::string& username, const std::string& oldPass,
                                       const std::string& newPass, const std::string& confPass,
                                       std::string& errorMsg) {
    QSqlQuery q;
    q.prepare("SELECT password FROM accounts WHERE username = :u");
    q.bindValue(":u", QString::fromStdString(username));
    q.exec();
    if (!q.next()) { errorMsg = "Account not found!"; return false; }

    if (q.value("password").toString() != hashPassword(oldPass)) {
        errorMsg = "Current password is incorrect!"; return false;
    }
    if (newPass != confPass) { errorMsg = "Password confirmation does not match!"; return false; }
    if (newPass == oldPass) { errorMsg = "New password must be different from current password!"; return false; }

    bool hasUpper = std::any_of(newPass.begin(), newPass.end(), [](unsigned char c){ return std::isupper(c); });
    bool hasLower = std::any_of(newPass.begin(), newPass.end(), [](unsigned char c){ return std::islower(c); });
    bool hasDigit = std::any_of(newPass.begin(), newPass.end(), [](unsigned char c){ return std::isdigit(c); });
    if (newPass.length() < 8 || !hasUpper || !hasLower || !hasDigit) {
        errorMsg = "New password must have at least 8 characters, including uppercase, lowercase, and numbers!";
        return false;
    }

    QSqlQuery upd;
    upd.prepare("UPDATE accounts SET password = :p WHERE username = :u");
    upd.bindValue(":p", hashPassword(newPass));
    upd.bindValue(":u", QString::fromStdString(username));
    if (!upd.exec()) { errorMsg = "Database Error: " + upd.lastError().text().toStdString(); return false; }
    return true;
}

bool StudentController::addStudent(const std::string& id, const std::string& name, const std::string& email,
                                   const std::string& phone, const std::string& classCode,
                                   const std::string& dateOfBirth, const std::string& gender,
                                   const std::string& address, std::string& errorMsg) {
    if (id.empty() || name.empty() || email.empty() || phone.empty() || classCode.empty() ||
        dateOfBirth.empty() || gender.empty() || address.empty()) {
        errorMsg = "Please enter all information!"; return false;
    }

    std::regex idRegex("^ST\\d{6}$");
    if (!std::regex_match(id, idRegex)) {
        errorMsg = "Invalid Student ID! (Example: ST000001)."; return false;
    }
    if (std::any_of(name.begin(), name.end(), ::isdigit)) {
        errorMsg = "Full name must not contain numbers!"; return false;
    }
    std::regex phoneRegex("^\\d{10}$");
    if (!std::regex_match(phone, phoneRegex)) {
        errorMsg = "Phone number must contain exactly 10 digits!"; return false;
    }
    std::regex emailRegex(R"(^[a-zA-Z0-9_.-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$)");
    if (!std::regex_match(email, emailRegex)) {
        errorMsg = "Invalid Email format!"; return false;
    }

    QDate dob = QDate::fromString(QString::fromStdString(dateOfBirth), "yyyy-MM-dd");
    if (!dob.isValid()) { errorMsg = "Invalid date of birth! (Format: YYYY-MM-DD)"; return false; }
    if (dob > QDate::currentDate()) { errorMsg = "Date of birth cannot be in the future!"; return false; }

    if (!classExists(classCode)) {
        errorMsg = "Class Code does not exist! Please create class first in Class Management."; return false;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM students WHERE id = :id");
    checkQuery.bindValue(":id", QString::fromStdString(id));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "Student ID already exists!"; return false; }

    QSqlQuery emailCheck;
    emailCheck.prepare("SELECT id FROM students WHERE email = :email");
    emailCheck.bindValue(":email", QString::fromStdString(email));
    emailCheck.exec();
    if (emailCheck.next()) { errorMsg = "This email is already in use by another student!"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO students (id, name, email, phone, classCode, dateOfBirth, gender, address) "
                  "VALUES (:id, :name, :email, :phone, :classCode, :dob, :gender, :address)");
    query.bindValue(":id", QString::fromStdString(id));
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":email", QString::fromStdString(email));
    query.bindValue(":phone", QString::fromStdString(phone));
    query.bindValue(":classCode", QString::fromStdString(classCode));
    query.bindValue(":dob", QString::fromStdString(dateOfBirth));
    query.bindValue(":gender", QString::fromStdString(gender));
    query.bindValue(":address", QString::fromStdString(address));

    if (!query.exec()) {
        errorMsg = "Database Error: " + query.lastError().text().toStdString();
        return false;
    }
    return true;
}

bool StudentController::updateStudent(const std::string& id, const std::string& name, const std::string& email,
                                      const std::string& phone, const std::string& classCode,
                                      const std::string& dateOfBirth, const std::string& gender,
                                      const std::string& address, std::string& errorMsg) {
    if (name.empty() || email.empty() || phone.empty() || classCode.empty() ||
        dateOfBirth.empty() || gender.empty() || address.empty()) {
        errorMsg = "Please enter all information!"; return false;
    }
    if (std::any_of(name.begin(), name.end(), ::isdigit)) {
        errorMsg = "Full name must not contain numbers!"; return false;
    }
    std::regex phoneRegex("^\\d{10}$");
    if (!std::regex_match(phone, phoneRegex)) {
        errorMsg = "Phone number must contain exactly 10 digits!"; return false;
    }
    std::regex emailRegex(R"(^[a-zA-Z0-9_.-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$)");
    if (!std::regex_match(email, emailRegex)) {
        errorMsg = "Invalid Email format!"; return false;
    }
    QDate dob = QDate::fromString(QString::fromStdString(dateOfBirth), "yyyy-MM-dd");
    if (!dob.isValid()) { errorMsg = "Invalid date of birth! (Format: YYYY-MM-DD)"; return false; }
    if (dob > QDate::currentDate()) { errorMsg = "Date of birth cannot be in the future!"; return false; }
    if (!classExists(classCode)) {
        errorMsg = "Class Code does not exist! Please create class first in Class Management."; return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE students SET name = :name, email = :email, phone = :phone, "
                  "classCode = :classCode, dateOfBirth = :dob, gender = :gender, address = :address "
                  "WHERE id = :id");
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":email", QString::fromStdString(email));
    query.bindValue(":phone", QString::fromStdString(phone));
    query.bindValue(":classCode", QString::fromStdString(classCode));
    query.bindValue(":dob", QString::fromStdString(dateOfBirth));
    query.bindValue(":gender", QString::fromStdString(gender));
    query.bindValue(":address", QString::fromStdString(address));
    query.bindValue(":id", QString::fromStdString(id));

    if (!query.exec() || query.numRowsAffected() == 0) {
        errorMsg = "Database Error or student not found!";
        return false;
    }
    return true;
}

bool StudentController::deleteStudent(const std::string& id) {
    QSqlQuery query;
    query.prepare("DELETE FROM students WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(id));
    return query.exec() && query.numRowsAffected() > 0;
}

std::vector<Student> StudentController::getAllStudents() {
    std::vector<Student> list;
    QSqlQuery query("SELECT * FROM students");
    while (query.next()) {
        list.push_back(Student(
            query.value("id").toString().toStdString(),
            query.value("name").toString().toStdString(),
            query.value("email").toString().toStdString(),
            query.value("phone").toString().toStdString(),
            query.value("classCode").toString().toStdString(),
            query.value("dateOfBirth").toString().toStdString(),
            query.value("gender").toString().toStdString(),
            query.value("address").toString().toStdString()
            ));
    }
    return list;
}

std::vector<Student> StudentController::searchStudents(const std::string& keyword) {
    std::vector<Student> list;
    QString kw = QString::fromStdString(keyword);
    QString like = "%" + kw + "%";

    QSqlQuery q;
    q.prepare("SELECT * FROM students WHERE id = :exact OR name LIKE :like "
              "OR email LIKE :like OR phone LIKE :like OR classCode LIKE :like");
    q.bindValue(":exact", kw);
    q.bindValue(":like", like);
    q.exec();

    while (q.next()) {
        list.push_back(Student(
            q.value("id").toString().toStdString(),
            q.value("name").toString().toStdString(),
            q.value("email").toString().toStdString(),
            q.value("phone").toString().toStdString(),
            q.value("classCode").toString().toStdString(),
            q.value("dateOfBirth").toString().toStdString(),
            q.value("gender").toString().toStdString(),
            q.value("address").toString().toStdString()
            ));
    }
    return list;
}

bool StudentController::addClass(const std::string& classCode, const std::string& className, std::string& errorMsg) {
    if (classCode.empty() || className.empty()) { errorMsg = "Please enter all class information!"; return false; }
    if (classExists(classCode)) { errorMsg = "Class Code already exists"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO classes (classCode, className) VALUES (:code, :name)");
    query.bindValue(":code", QString::fromStdString(classCode));
    query.bindValue(":name", QString::fromStdString(className));
    return query.exec();
}

bool StudentController::addSubject(const std::string& subjectCode, const std::string& subjectName, int credits, std::string& errorMsg) {
    if (subjectCode.empty() || subjectName.empty()) { errorMsg = "Please enter all subject information!"; return false; }
    if (credits <= 0) { errorMsg = "Credits must be greater than 0"; return false; }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT subjectCode FROM subjects WHERE subjectCode = :code");
    checkQuery.bindValue(":code", QString::fromStdString(subjectCode));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "Subject Code already exists!"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO subjects (subjectCode, subjectName, credits) VALUES (:code, :name, :credits)");
    query.bindValue(":code", QString::fromStdString(subjectCode));
    query.bindValue(":name", QString::fromStdString(subjectName));
    query.bindValue(":credits", credits);
    return query.exec();
}

bool StudentController::enrollStudent(const std::string& studentId, const std::string& subjectCode, std::string& errorMsg) {
    QSqlQuery studentCheck;
    studentCheck.prepare("SELECT id FROM students WHERE id = :id");
    studentCheck.bindValue(":id", QString::fromStdString(studentId));
    studentCheck.exec();
    if (!studentCheck.next()) { errorMsg = "Student not found!"; return false; }

    QSqlQuery subjectCheck;
    subjectCheck.prepare("SELECT subjectCode FROM subjects WHERE subjectCode = :code");
    subjectCheck.bindValue(":code", QString::fromStdString(subjectCode));
    subjectCheck.exec();
    if (!subjectCheck.next()) { errorMsg = "Subject not found!"; return false; }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM enrollments WHERE studentId = :stuId AND subjectCode = :subCode");
    checkQuery.bindValue(":stuId", QString::fromStdString(studentId));
    checkQuery.bindValue(":subCode", QString::fromStdString(subjectCode));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "Duplicate enrollment is not allowed"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO enrollments (studentId, subjectCode) VALUES (:stuId, :subCode)");
    query.bindValue(":stuId", QString::fromStdString(studentId));
    query.bindValue(":subCode", QString::fromStdString(subjectCode));
    return query.exec();
}

bool StudentController::dropEnrollment(int enrollmentId, std::string& errorMsg) {
    QSqlQuery check;
    check.prepare("SELECT score FROM enrollments WHERE id = :id");
    check.bindValue(":id", enrollmentId);
    check.exec();
    if (!check.next()) { errorMsg = "Enrollment not found!"; return false; }

    double score = check.value("score").toDouble();
    if (score >= 0) { errorMsg = "Cannot drop a graded subject!"; return false; }

    QSqlQuery del;
    del.prepare("DELETE FROM enrollments WHERE id = :id");
    del.bindValue(":id", enrollmentId);
    if (!del.exec()) { errorMsg = "Database Error: " + del.lastError().text().toStdString(); return false; }
    return true;
}

std::vector<ClassInfo> StudentController::getAllClasses() {
    std::vector<ClassInfo> list;
    QSqlQuery q("SELECT classCode, className FROM classes ORDER BY classCode");
    while (q.next()) {
        list.push_back({ q.value("classCode").toString().toStdString(),
                        q.value("className").toString().toStdString() });
    }
    return list;
}

std::vector<SubjectInfo> StudentController::getAllSubjects() {
    std::vector<SubjectInfo> list;
    QSqlQuery q("SELECT subjectCode, subjectName, credits FROM subjects ORDER BY subjectCode");
    while (q.next()) {
        list.push_back({ q.value("subjectCode").toString().toStdString(),
                        q.value("subjectName").toString().toStdString(),
                        q.value("credits").toInt() });
    }
    return list;
}

std::vector<SubjectInfo> StudentController::getAvailableSubjects(const std::string& studentId) {
    std::vector<SubjectInfo> list;
    QSqlQuery q;
    q.prepare(
        "SELECT subjectCode, subjectName, credits FROM subjects "
        "WHERE subjectCode NOT IN (SELECT subjectCode FROM enrollments WHERE studentId = :id) "
        "ORDER BY subjectCode");
    q.bindValue(":id", QString::fromStdString(studentId));
    q.exec();
    while (q.next()) {
        list.push_back({ q.value("subjectCode").toString().toStdString(),
                        q.value("subjectName").toString().toStdString(),
                        q.value("credits").toInt() });
    }
    return list;
}

std::vector<EnrollmentInfo> StudentController::getAllEnrollments() {
    std::vector<EnrollmentInfo> list;
    QSqlQuery q(
        "SELECT e.id, e.studentId, s.name AS studentName, e.subjectCode, "
        "       sub.subjectName, e.score "
        "FROM enrollments e "
        "JOIN students s ON s.id = e.studentId "
        "JOIN subjects sub ON sub.subjectCode = e.subjectCode "
        "ORDER BY e.id DESC");
    while (q.next()) {
        EnrollmentInfo e;
        e.id = q.value("id").toInt();
        e.studentId = q.value("studentId").toString().toStdString();
        e.studentName = q.value("studentName").toString().toStdString();
        e.subjectCode = q.value("subjectCode").toString().toStdString();
        e.subjectName = q.value("subjectName").toString().toStdString();
        e.score = q.value("score").toDouble();
        list.push_back(e);
    }
    return list;
}

std::vector<GradeRow> StudentController::getTeacherGradebook(const std::string& teacherUsername) {
    std::vector<GradeRow> list;
    QSqlQuery q;
    q.prepare(
        "SELECT s.id AS studentId, s.name AS studentName, sub.subjectCode, sub.subjectName, "
        "       e.id AS enrollmentId, e.score "
        "FROM assignments a "
        "JOIN students s ON s.classCode = a.classCode "
        "JOIN subjects sub ON sub.subjectCode = a.subjectCode "
        "LEFT JOIN enrollments e ON e.studentId = s.id AND e.subjectCode = a.subjectCode "
        "WHERE a.teacherName = :t "
        "ORDER BY sub.subjectName, s.name");
    q.bindValue(":t", QString::fromStdString(teacherUsername));
    q.exec();

    while (q.next()) {
        GradeRow r;
        r.studentId = q.value("studentId").toString().toStdString();
        r.studentName = q.value("studentName").toString().toStdString();
        r.subjectCode = q.value("subjectCode").toString().toStdString();
        r.subjectName = q.value("subjectName").toString().toStdString();
        QVariant enr = q.value("enrollmentId");
        r.enrollmentId = enr.isNull() ? -1 : enr.toInt();
        QVariant sc = q.value("score");
        r.score = sc.isNull() ? -1.0 : sc.toDouble();
        list.push_back(r);
    }
    return list;
}

bool StudentController::updateGrade(int enrollmentId, double score, std::string& errorMsg) {
    if (enrollmentId < 0) {
        errorMsg = "Student is not enrolled in this subject, cannot grade!"; return false;
    }
    if (score < 0.0 || score > 10.0) {
        errorMsg = "Invalid score! Please enter between 0.0 and 10.0"; return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE enrollments SET score = :score WHERE id = :id");
    query.bindValue(":score", score);
    query.bindValue(":id", enrollmentId);

    if (!query.exec() || query.numRowsAffected() == 0) {
        errorMsg = "Enrollment not found!"; return false;
    }
    return true;
}

bool StudentController::assignTeacher(const std::string& teacherName, const std::string& classCode,
                                      const std::string& subjectCode, std::string& errorMsg) {
    if (teacherName.empty() || classCode.empty() || subjectCode.empty()) {
        errorMsg = "Please enter all assignment information!"; return false;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM assignments WHERE teacherName = :t AND classCode = :c AND subjectCode = :s");
    checkQuery.bindValue(":t", QString::fromStdString(teacherName));
    checkQuery.bindValue(":c", QString::fromStdString(classCode));
    checkQuery.bindValue(":s", QString::fromStdString(subjectCode));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "This assignment already exists!"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO assignments (teacherName, classCode, subjectCode) VALUES (:teacher, :class, :subject)");
    query.bindValue(":teacher", QString::fromStdString(teacherName));
    query.bindValue(":class", QString::fromStdString(classCode));
    query.bindValue(":subject", QString::fromStdString(subjectCode));
    return query.exec();
}

std::vector<AssignmentInfo> StudentController::getAllAssignments() {
    std::vector<AssignmentInfo> list;
    QSqlQuery q("SELECT id, teacherName, classCode, subjectCode FROM assignments ORDER BY id DESC");
    while (q.next()) {
        AssignmentInfo a;
        a.id = q.value("id").toInt();
        a.teacherName = q.value("teacherName").toString().toStdString();
        a.classCode = q.value("classCode").toString().toStdString();
        a.subjectCode = q.value("subjectCode").toString().toStdString();
        list.push_back(a);
    }
    return list;
}

bool StudentController::addAccount(const std::string& username, const std::string& password,
                                   const std::string& role, std::string& errorMsg) {
    if (username.empty() || password.empty() || role.empty()) {
        errorMsg = "Please enter all account information!"; return false;
    }
    if (role != "admin" && role != "teacher" && role != "student") {
        errorMsg = "Invalid role! (admin / teacher / student)"; return false;
    }
    if (password.length() < 8) {
        errorMsg = "Password must be at least 8 characters!"; return false;
    }

    QSqlQuery check;
    check.prepare("SELECT username FROM accounts WHERE username = :u");
    check.bindValue(":u", QString::fromStdString(username));
    check.exec();
    if (check.next()) { errorMsg = "Username already exists!"; return false; }

    QSqlQuery ins;
    ins.prepare("INSERT INTO accounts (username, password, role) VALUES (:u, :p, :r)");
    ins.bindValue(":u", QString::fromStdString(username));
    ins.bindValue(":p", hashPassword(password));
    ins.bindValue(":r", QString::fromStdString(role));
    if (!ins.exec()) { errorMsg = "Database Error: " + ins.lastError().text().toStdString(); return false; }
    return true;
}

std::vector<std::pair<std::string, std::string>> StudentController::getAllAccounts() {
    std::vector<std::pair<std::string, std::string>> list;
    QSqlQuery q("SELECT username, role FROM accounts ORDER BY username");
    while (q.next()) {
        list.push_back({ q.value("username").toString().toStdString(),
                        q.value("role").toString().toStdString() });
    }
    return list;
}

StudentProfile StudentController::getStudentProfile(const std::string& username) {
    StudentProfile p;
    QSqlQuery q;
    q.prepare("SELECT * FROM students WHERE id = :id");
    q.bindValue(":id", QString::fromStdString(username));
    q.exec();
    if (q.next()) {
        p.found = true;
        p.id = q.value("id").toString().toStdString();
        p.name = q.value("name").toString().toStdString();
        p.email = q.value("email").toString().toStdString();
        p.phone = q.value("phone").toString().toStdString();
        p.classCode = q.value("classCode").toString().toStdString();
        p.dateOfBirth = q.value("dateOfBirth").toString().toStdString();
        p.gender = q.value("gender").toString().toStdString();
        p.address = q.value("address").toString().toStdString();
    }
    return p;
}

std::vector<SubjectResult> StudentController::getStudentResults(const std::string& username) {
    std::vector<SubjectResult> list;
    QSqlQuery q;
    q.prepare("SELECT sub.subjectName, sub.credits, e.score, e.id AS enrollmentId "
              "FROM enrollments e JOIN subjects sub ON sub.subjectCode = e.subjectCode "
              "WHERE e.studentId = :id");
    q.bindValue(":id", QString::fromStdString(username));
    q.exec();
    while (q.next()) {
        SubjectResult r;
        r.subjectName = q.value("subjectName").toString().toStdString();
        r.credits = q.value("credits").toInt();
        r.score = q.value("score").toDouble();
        r.enrollmentId = q.value("enrollmentId").toInt();
        list.push_back(r);
    }
    return list;
}

double StudentController::computeGPA(const std::vector<SubjectResult>& results) {
    double totalPoints = 0;
    int totalCredits = 0;
    for (const auto& r : results) {
        if (r.score < 0) continue;
        totalPoints += r.score * r.credits;
        totalCredits += r.credits;
    }
    if (totalCredits == 0) return -1.0;
    return totalPoints / totalCredits;
}

std::unique_ptr<User> StudentController::buildSessionUser(int role, const std::string& username) {
    if (role == 1) {
        return std::make_unique<Admin>(username, "", username, "");
    }

    if (role == 2) {
        QSqlQuery q;
        q.prepare("SELECT fullName, email FROM teachers WHERE username = :u");
        q.bindValue(":u", QString::fromStdString(username));
        q.exec();
        std::string fullName = username;
        std::string email;
        if (q.next()) {
            fullName = q.value("fullName").toString().toStdString();
            email = q.value("email").toString().toStdString();
        }
        return std::make_unique<Teacher>(username, "", fullName, email);
    }

    if (role == 3) {
        StudentProfile p = getStudentProfile(username);
        return std::make_unique<StudentUser>(username, "",
                                             p.found ? p.id : username,
                                             p.found ? p.name : username);
    }

    return nullptr;
}