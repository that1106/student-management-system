#include "StudentController.h"
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
               "('teacher01', 'Nguyen Van A', '[email protected]')");
    else q.exec("INSERT IGNORE INTO teachers (username, fullName, email) VALUES "
               "('teacher01', 'Nguyen Van A', '[email protected]')");

    if (isSqlite) q.exec("INSERT OR IGNORE INTO classes (classCode, className) VALUES "
               "('ST101', 'Lop Ky Thuat Phan Mem 101')");
    else q.exec("INSERT IGNORE INTO classes (classCode, className) VALUES "
               "('ST101', 'Lop Ky Thuat Phan Mem 101')");

    if (isSqlite) {
        q.exec("INSERT OR IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('MATH101', 'Toan Cao Cap', 3)");
        q.exec("INSERT OR IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('PRO101', 'Lap Trinh Huong Doi Tuong', 4)");
    } else {
        q.exec("INSERT IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('MATH101', 'Toan Cao Cap', 3)");
        q.exec("INSERT IGNORE INTO subjects (subjectCode, subjectName, credits) VALUES "
               "('PRO101', 'Lap Trinh Huong Doi Tuong', 4)");
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
        ins.bindValue(":email", "[email protected]");
        ins.bindValue(":phone", "0900000002");
        ins.bindValue(":class", "ST101");
        ins.bindValue(":dob", "2004-05-10");
        ins.bindValue(":gender", "Nam");
        ins.bindValue(":address", "TP. Ho Chi Minh");
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
    if (!q.next()) { errorMsg = "Khong tim thay tai khoan!"; return false; }

    if (q.value("password").toString() != hashPassword(oldPass)) {
        errorMsg = "Mat khau hien tai khong dung!"; return false;
    }
    if (newPass != confPass) { errorMsg = "Mat khau xac nhan khong khop!"; return false; }
    if (newPass == oldPass) { errorMsg = "Mat khau moi phai khac mat khau hien tai!"; return false; }

    bool hasUpper = std::any_of(newPass.begin(), newPass.end(), [](unsigned char c){ return std::isupper(c); });
    bool hasLower = std::any_of(newPass.begin(), newPass.end(), [](unsigned char c){ return std::islower(c); });
    bool hasDigit = std::any_of(newPass.begin(), newPass.end(), [](unsigned char c){ return std::isdigit(c); });
    if (newPass.length() < 8 || !hasUpper || !hasLower || !hasDigit) {
        errorMsg = "Mat khau moi phai co it nhat 8 ky tu, gom chu hoa, chu thuong va chu so!";
        return false;
    }

    QSqlQuery upd;
    upd.prepare("UPDATE accounts SET password = :p WHERE username = :u");
    upd.bindValue(":p", hashPassword(newPass));
    upd.bindValue(":u", QString::fromStdString(username));
    if (!upd.exec()) { errorMsg = "Loi Database: " + upd.lastError().text().toStdString(); return false; }
    return true;
}



bool StudentController::addStudent(const std::string& id, const std::string& name, const std::string& email,
                                   const std::string& phone, const std::string& classCode,
                                   const std::string& dateOfBirth, const std::string& gender,
                                   const std::string& address, std::string& errorMsg) {
    if (id.empty() || name.empty() || email.empty() || phone.empty() || classCode.empty() ||
        dateOfBirth.empty() || gender.empty() || address.empty()) {
        errorMsg = "Vui long nhap day du thong tin!"; return false;
    }

    std::regex idRegex("^ST\\d{6}$");
    if (!std::regex_match(id, idRegex)) {
        errorMsg = "Ma SV khong hop le! (Vi du: ST000001)."; return false;
    }
    if (std::any_of(name.begin(), name.end(), ::isdigit)) {
        errorMsg = "Ho va ten khong duoc chua chu so!"; return false;
    }
    std::regex phoneRegex("^\\d{10}$");
    if (!std::regex_match(phone, phoneRegex)) {
        errorMsg = "So dien thoai phai bao gom dung 10 chu so!"; return false;
    }
    std::regex emailRegex(R"(^[a-zA-Z0-9_.-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$)");
    if (!std::regex_match(email, emailRegex)) {
        errorMsg = "Dinh dang Email khong hop le!"; return false;
    }

    QDate dob = QDate::fromString(QString::fromStdString(dateOfBirth), "yyyy-MM-dd");
    if (!dob.isValid()) { errorMsg = "Ngay sinh khong hop le! (Dinh dang: YYYY-MM-DD)"; return false; }
    if (dob > QDate::currentDate()) { errorMsg = "Ngay sinh khong duoc lon hon ngay hien tai!"; return false; }

    if (!classExists(classCode)) {
        errorMsg = "Ma lop khong ton tai! Vui long tao lop truoc trong Quan ly Lop."; return false;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM students WHERE id = :id");
    checkQuery.bindValue(":id", QString::fromStdString(id));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "Ma sinh vien da ton tai!"; return false; }

    QSqlQuery emailCheck;
    emailCheck.prepare("SELECT id FROM students WHERE email = :email");
    emailCheck.bindValue(":email", QString::fromStdString(email));
    emailCheck.exec();
    if (emailCheck.next()) { errorMsg = "Email nay da duoc su dung boi sinh vien khac!"; return false; }

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
        errorMsg = "Loi Database: " + query.lastError().text().toStdString();
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
        errorMsg = "Vui long nhap day du thong tin!"; return false;
    }
    if (std::any_of(name.begin(), name.end(), ::isdigit)) {
        errorMsg = "Ho va ten khong duoc chua chu so!"; return false;
    }
    std::regex phoneRegex("^\\d{10}$");
    if (!std::regex_match(phone, phoneRegex)) {
        errorMsg = "So dien thoai phai bao gom dung 10 chu so!"; return false;
    }
    std::regex emailRegex(R"(^[a-zA-Z0-9_.-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$)");
    if (!std::regex_match(email, emailRegex)) {
        errorMsg = "Dinh dang Email khong hop le!"; return false;
    }
    QDate dob = QDate::fromString(QString::fromStdString(dateOfBirth), "yyyy-MM-dd");
    if (!dob.isValid()) { errorMsg = "Ngay sinh khong hop le! (Dinh dang: YYYY-MM-DD)"; return false; }
    if (dob > QDate::currentDate()) { errorMsg = "Ngay sinh khong duoc lon hon ngay hien tai!"; return false; }
    if (!classExists(classCode)) {
        errorMsg = "Ma lop khong ton tai! Vui long tao lop truoc trong Quan ly Lop."; return false;
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
        errorMsg = "Loi Database hoac khong tim thay sinh vien!";
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
    if (classCode.empty() || className.empty()) { errorMsg = "Vui long nhap du thong tin lop!"; return false; }
    if (classExists(classCode)) { errorMsg = "Ma lop da ton tai!"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO classes (classCode, className) VALUES (:code, :name)");
    query.bindValue(":code", QString::fromStdString(classCode));
    query.bindValue(":name", QString::fromStdString(className));
    return query.exec();
}

bool StudentController::addSubject(const std::string& subjectCode, const std::string& subjectName, int credits, std::string& errorMsg) {
    if (subjectCode.empty() || subjectName.empty()) { errorMsg = "Vui long nhap du thong tin mon hoc!"; return false; }
    if (credits <= 0) { errorMsg = "So tin chi phai lon hon 0!"; return false; }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT subjectCode FROM subjects WHERE subjectCode = :code");
    checkQuery.bindValue(":code", QString::fromStdString(subjectCode));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "Ma mon hoc da ton tai!"; return false; }

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
    if (!studentCheck.next()) { errorMsg = "Khong tim thay sinh vien!"; return false; }

    QSqlQuery subjectCheck;
    subjectCheck.prepare("SELECT subjectCode FROM subjects WHERE subjectCode = :code");
    subjectCheck.bindValue(":code", QString::fromStdString(subjectCode));
    subjectCheck.exec();
    if (!subjectCheck.next()) { errorMsg = "Khong tim thay mon hoc!"; return false; }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM enrollments WHERE studentId = :stuId AND subjectCode = :subCode");
    checkQuery.bindValue(":stuId", QString::fromStdString(studentId));
    checkQuery.bindValue(":subCode", QString::fromStdString(subjectCode));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "Sinh vien nay da dang ky mon hoc nay roi!"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO enrollments (studentId, subjectCode) VALUES (:stuId, :subCode)");
    query.bindValue(":stuId", QString::fromStdString(studentId));
    query.bindValue(":subCode", QString::fromStdString(subjectCode));
    return query.exec();
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
        errorMsg = "Sinh vien chua dang ky mon hoc nay, khong the cham diem!"; return false;
    }
    if (score < 0.0 || score > 10.0) {
        errorMsg = "Diem so khong hop le! Vui long nhap tu 0.0 den 10.0"; return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE enrollments SET score = :score WHERE id = :id");
    query.bindValue(":score", score);
    query.bindValue(":id", enrollmentId);

    if (!query.exec() || query.numRowsAffected() == 0) {
        errorMsg = "Khong tim thay du lieu dang ky nay!"; return false;
    }
    return true;
}

bool StudentController::assignTeacher(const std::string& teacherName, const std::string& classCode,
                                      const std::string& subjectCode, std::string& errorMsg) {
    if (teacherName.empty() || classCode.empty() || subjectCode.empty()) {
        errorMsg = "Vui long nhap du thong tin phan cong!"; return false;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM assignments WHERE teacherName = :t AND classCode = :c AND subjectCode = :s");
    checkQuery.bindValue(":t", QString::fromStdString(teacherName));
    checkQuery.bindValue(":c", QString::fromStdString(classCode));
    checkQuery.bindValue(":s", QString::fromStdString(subjectCode));
    checkQuery.exec();
    if (checkQuery.next()) { errorMsg = "Phan cong nay da ton tai!"; return false; }

    QSqlQuery query;
    query.prepare("INSERT INTO assignments (teacherName, classCode, subjectCode) VALUES (:teacher, :class, :subject)");
    query.bindValue(":teacher", QString::fromStdString(teacherName));
    query.bindValue(":class", QString::fromStdString(classCode));
    query.bindValue(":subject", QString::fromStdString(subjectCode));
    return query.exec();
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
        if (r.score < 0) continue; // not graded yet
        totalPoints += r.score * r.credits;
        totalCredits += r.credits;
    }
    if (totalCredits == 0) return -1.0;
    return totalPoints / totalCredits;
}