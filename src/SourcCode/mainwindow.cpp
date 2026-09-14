#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    this->setWindowTitle("Student Management System - Role Based Access");
    this->resize(1150, 780);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    loginWidget = new QWidget();
    adminWidget = new QWidget();
    teacherWidget = new QWidget();
    studentWidget = new QWidget();

    setupLoginUI();
    setupAdminUI();
    setupTeacherUI();
    setupStudentUI();

    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(adminWidget);
    stackedWidget->addWidget(teacherWidget);
    stackedWidget->addWidget(studentWidget);

    stackedWidget->setCurrentIndex(0);

    this->setStyleSheet(R"(
        QWidget { background-color: #1e1e2e; color: #cdd6f4; font-family: 'Segoe UI'; font-size: 14px; }
        QLabel { color: #89b4fa; font-weight: bold; }
        QLineEdit, QComboBox { background-color: #313244; border: 1px solid #45475a; border-radius: 8px; padding: 8px; color: #cdd6f4; }
        QPushButton { background-color: #89b4fa; color: #11111b; border-radius: 8px; padding: 10px; font-weight: bold; }
        QPushButton:hover { background-color: #b4befe; }
        QTableWidget { background-color: #1e1e2e; border: 1px solid #45475a; border-radius: 8px; }
        QHeaderView::section { background-color: #313244; color: #cdd6f4; padding: 6px; }
    )");
}

void MainWindow::setupLoginUI() {
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);
    QLabel *title = new QLabel("<h2>STUDENT MANAGEMENT SYSTEM</h2>");
    title->setAlignment(Qt::AlignCenter);

    txtUser = new QLineEdit();
    txtUser->setPlaceholderText("Username (admin / teacher01 / ST000002)");

    txtPass = new QLineEdit();
    txtPass->setPlaceholderText("Password");
    txtPass->setEchoMode(QLineEdit::Password);

    btnLogin = new QPushButton("LOGIN");

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(txtUser);
    layout->addWidget(txtPass);
    layout->addWidget(btnLogin);
    layout->addStretch();

    connect(btnLogin, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(txtPass, &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
}

void MainWindow::onLoginClicked() {
    std::string username = txtUser->text().toStdString();
    int role = controller.login(username, txtPass->text().toStdString());

    if (role != 0) {
        currentUsername = username;
        currentUser = controller.buildSessionUser(role, username);
    }

    if (role == 1) {
        stackedWidget->setCurrentIndex(1);
        lblWelcomeAdmin->setText(QString::fromStdString(currentUser->welcomeMessage()));
        refreshAdminAll();
    } else if (role == 2) {
        stackedWidget->setCurrentIndex(2);
        lblWelcomeTeacher->setText(QString::fromStdString(currentUser->welcomeMessage()));
        refreshTeacherView();
    } else if (role == 3) {
        stackedWidget->setCurrentIndex(3);
        lblWelcomeStudent->setText(QString::fromStdString(currentUser->welcomeMessage()));
        refreshStudentView();
    } else {
        QMessageBox::warning(this, "Error", "Invalid username or password!");
    }
    txtUser->clear();
    txtPass->clear();
}

void MainWindow::setupAdminUI() {
    QVBoxLayout *outer = new QVBoxLayout(adminWidget);
    QLabel *lbl = new QLabel("<h3>SYSTEM ADMINISTRATION - ADMIN WORKSPACE</h3>");
    outer->addWidget(lbl);
    lblWelcomeAdmin = new QLabel("...");
    outer->addWidget(lblWelcomeAdmin);

    adminTabs = new QTabWidget();
    outer->addWidget(adminTabs);

    QWidget *tabStudents = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tabStudents);

    QGridLayout *formLayout = new QGridLayout();
    txtId = new QLineEdit(); txtId->setPlaceholderText("Student ID (e.g. ST000001)");
    txtName = new QLineEdit(); txtName->setPlaceholderText("Full Name");
    txtEmail = new QLineEdit(); txtEmail->setPlaceholderText("Email");
    txtPhone = new QLineEdit(); txtPhone->setPlaceholderText("Phone Number (10 digits)");
    txtClass = new QLineEdit(); txtClass->setPlaceholderText("Class Code (e.g. ST101)");
    txtDob = new QLineEdit(); txtDob->setPlaceholderText("Date of Birth (YYYY-MM-DD)");
    cmbGender = new QComboBox(); cmbGender->addItems({"Male", "Female", "Other"});
    txtAddress = new QLineEdit(); txtAddress->setPlaceholderText("Address");

    formLayout->addWidget(txtId, 0, 0);
    formLayout->addWidget(txtName, 0, 1);
    formLayout->addWidget(txtEmail, 0, 2);
    formLayout->addWidget(txtPhone, 1, 0);
    formLayout->addWidget(txtClass, 1, 1);
    formLayout->addWidget(txtDob, 1, 2);
    formLayout->addWidget(cmbGender, 2, 0);
    formLayout->addWidget(txtAddress, 2, 1, 1, 2);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Add");
    btnUpdate = new QPushButton("Update");
    btnDelete = new QPushButton("Delete");
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnUpdate);
    btnLayout->addWidget(btnDelete);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    txtSearch = new QLineEdit(); txtSearch->setPlaceholderText("Enter ID, Name, Email, Phone or Class to search...");
    btnSearch = new QPushButton("Search");
    searchLayout->addWidget(txtSearch);
    searchLayout->addWidget(btnSearch);

    adminTable = new QTableWidget(0, 8);
    adminTable->setHorizontalHeaderLabels({"Student ID", "Full Name", "Email", "Phone", "Class", "Date of Birth", "Gender", "Address"});
    adminTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addLayout(formLayout);
    layout->addLayout(btnLayout);
    layout->addLayout(searchLayout);
    layout->addWidget(adminTable);

    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(txtSearch, &QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddStudentClicked);
    connect(btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateStudentClicked);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteStudentClicked);
    connect(adminTable, &QTableWidget::cellClicked, this, &MainWindow::onTableClicked);

    adminTabs->addTab(tabStudents, "Students");

    QWidget *tabClassSubject = new QWidget();
    QVBoxLayout *csLayout = new QVBoxLayout(tabClassSubject);

    csLayout->addWidget(new QLabel("Class Management (FR-07)"));
    QHBoxLayout *classFormLayout = new QHBoxLayout();
    txtClassCode = new QLineEdit(); txtClassCode->setPlaceholderText("Class Code (e.g. ST101)");
    txtClassName = new QLineEdit(); txtClassName->setPlaceholderText("Class Name");
    btnAddClass = new QPushButton("Add Class");
    classFormLayout->addWidget(txtClassCode);
    classFormLayout->addWidget(txtClassName);
    classFormLayout->addWidget(btnAddClass);
    csLayout->addLayout(classFormLayout);

    classesTable = new QTableWidget(0, 2);
    classesTable->setHorizontalHeaderLabels({"Class Code", "Class Name"});
    classesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    csLayout->addWidget(classesTable);

    csLayout->addWidget(new QLabel("Subject Management (FR-08)"));
    QHBoxLayout *subjectFormLayout = new QHBoxLayout();
    txtSubjectCode = new QLineEdit(); txtSubjectCode->setPlaceholderText("Subject Code (e.g. PRF192)");
    txtSubjectName = new QLineEdit(); txtSubjectName->setPlaceholderText("Subject Name");
    txtCredits = new QLineEdit(); txtCredits->setPlaceholderText("Credits");
    btnAddSubject = new QPushButton("Add Subject");
    subjectFormLayout->addWidget(txtSubjectCode);
    subjectFormLayout->addWidget(txtSubjectName);
    subjectFormLayout->addWidget(txtCredits);
    subjectFormLayout->addWidget(btnAddSubject);
    csLayout->addLayout(subjectFormLayout);

    subjectsTable = new QTableWidget(0, 3);
    subjectsTable->setHorizontalHeaderLabels({"Subject Code", "Subject Name", "Credits"});
    subjectsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    csLayout->addWidget(subjectsTable);

    connect(btnAddClass, &QPushButton::clicked, this, &MainWindow::onAddClassClicked);
    connect(btnAddSubject, &QPushButton::clicked, this, &MainWindow::onAddSubjectClicked);

    adminTabs->addTab(tabClassSubject, "Classes & Subjects");

    QWidget *tabEnroll = new QWidget();
    QVBoxLayout *enrollLayout = new QVBoxLayout(tabEnroll);

    enrollLayout->addWidget(new QLabel("Course Registration for Students (FR-09)"));
    QHBoxLayout *enrollFormLayout = new QHBoxLayout();
    txtEnrollStudentId = new QLineEdit(); txtEnrollStudentId->setPlaceholderText("Student ID (e.g. ST000001)");
    txtEnrollSubjectCode = new QLineEdit(); txtEnrollSubjectCode->setPlaceholderText("Subject Code (e.g. PRF192)");
    btnEnroll = new QPushButton("Register");
    enrollFormLayout->addWidget(txtEnrollStudentId);
    enrollFormLayout->addWidget(txtEnrollSubjectCode);
    enrollFormLayout->addWidget(btnEnroll);
    enrollLayout->addLayout(enrollFormLayout);

    enrollmentsTable = new QTableWidget(0, 5);
    enrollmentsTable->setHorizontalHeaderLabels({"Student ID", "Full Name", "Subject Code", "Subject Name", "Score"});
    enrollmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    enrollLayout->addWidget(enrollmentsTable);

    connect(btnEnroll, &QPushButton::clicked, this, &MainWindow::onEnrollClicked);

    adminTabs->addTab(tabEnroll, "Enrollment");

    QWidget *tabAccounts = new QWidget();
    QVBoxLayout *accLayout = new QVBoxLayout(tabAccounts);

    accLayout->addWidget(new QLabel("Create Account (FR-15)"));
    QHBoxLayout *accFormLayout = new QHBoxLayout();
    txtAccUsername = new QLineEdit(); txtAccUsername->setPlaceholderText("Username");
    txtAccPassword = new QLineEdit(); txtAccPassword->setPlaceholderText("Password (>= 8 chars)");
    txtAccPassword->setEchoMode(QLineEdit::Password);
    cmbAccRole = new QComboBox(); cmbAccRole->addItems({"admin", "teacher", "student"});
    btnAddAccount = new QPushButton("Create Account");
    accFormLayout->addWidget(txtAccUsername);
    accFormLayout->addWidget(txtAccPassword);
    accFormLayout->addWidget(cmbAccRole);
    accFormLayout->addWidget(btnAddAccount);
    accLayout->addLayout(accFormLayout);

    accountsTable = new QTableWidget(0, 2);
    accountsTable->setHorizontalHeaderLabels({"Username", "Role"});
    accountsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    accLayout->addWidget(accountsTable);

    accLayout->addWidget(new QLabel("Teaching Assignment (FR-17)"));
    QHBoxLayout *assignFormLayout = new QHBoxLayout();
    txtAssignTeacher = new QLineEdit(); txtAssignTeacher->setPlaceholderText("Teacher Username (e.g. teacher01)");
    txtAssignClass = new QLineEdit(); txtAssignClass->setPlaceholderText("Class Code");
    txtAssignSubject = new QLineEdit(); txtAssignSubject->setPlaceholderText("Subject Code");
    btnAssignTeacher = new QPushButton("Assign");
    assignFormLayout->addWidget(txtAssignTeacher);
    assignFormLayout->addWidget(txtAssignClass);
    assignFormLayout->addWidget(txtAssignSubject);
    assignFormLayout->addWidget(btnAssignTeacher);
    accLayout->addLayout(assignFormLayout);

    assignmentsTable = new QTableWidget(0, 3);
    assignmentsTable->setHorizontalHeaderLabels({"Teacher", "Class", "Subject"});
    assignmentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    accLayout->addWidget(assignmentsTable);

    connect(btnAddAccount, &QPushButton::clicked, this, &MainWindow::onAddAccountClicked);
    connect(btnAssignTeacher, &QPushButton::clicked, this, &MainWindow::onAssignTeacherClicked);

    adminTabs->addTab(tabAccounts, "Accounts & Assignments");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    btnChangePassAdmin = new QPushButton("Change Password");
    btnLogoutAdmin = new QPushButton("Logout");
    bottomLayout->addWidget(btnChangePassAdmin);
    bottomLayout->addWidget(btnLogoutAdmin);
    outer->addLayout(bottomLayout);

    connect(btnLogoutAdmin, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(btnChangePassAdmin, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);
}

void MainWindow::setupTeacherUI() {
    QVBoxLayout *layout = new QVBoxLayout(teacherWidget);
    QLabel *lbl = new QLabel("<h3>TEACHER WORKSPACE</h3>");
    lblWelcomeTeacher = new QLabel("...");

    teacherTable = new QTableWidget(0, 4);
    teacherTable->setHorizontalHeaderLabels({"Student ID", "Full Name", "Subject", "Final Score"});
    teacherTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *gradeLayout = new QHBoxLayout();
    lblTeacherSelection = new QLabel("Select a row to grade");
    txtTeacherScore = new QLineEdit(); txtTeacherScore->setPlaceholderText("Score (0 - 10)");
    btnSaveGrade = new QPushButton("Save Grade");
    gradeLayout->addWidget(lblTeacherSelection, 1);
    gradeLayout->addWidget(txtTeacherScore);
    gradeLayout->addWidget(btnSaveGrade);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    btnChangePassTeacher = new QPushButton("Change Password");
    btnLogoutTeacher = new QPushButton("Logout");
    bottomLayout->addWidget(btnChangePassTeacher);
    bottomLayout->addWidget(btnLogoutTeacher);

    layout->addWidget(lbl);
    layout->addWidget(lblWelcomeTeacher);
    layout->addWidget(teacherTable);
    layout->addLayout(gradeLayout);
    layout->addLayout(bottomLayout);

    connect(btnLogoutTeacher, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(btnChangePassTeacher, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);
    connect(teacherTable, &QTableWidget::cellClicked, this, &MainWindow::onTeacherRowClicked);
    connect(btnSaveGrade, &QPushButton::clicked, this, &MainWindow::onSaveGradeClicked);
}

void MainWindow::setupStudentUI() {
    QVBoxLayout *layout = new QVBoxLayout(studentWidget);
    QLabel *lbl = new QLabel("<h3>ACADEMIC RESULTS - STUDENT WORKSPACE</h3>");
    lblWelcomeStudent = new QLabel("...");
    lblStudentInfo = new QLabel("...");
    lblGPA = new QLabel("...");

    studentTable = new QTableWidget(0, 3);
    studentTable->setHorizontalHeaderLabels({"Subject", "Credits", "Score"});
    studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    btnDropSubject = new QPushButton("Drop Selected Subject (only if ungraded)");

    QLabel *lblAvailable = new QLabel("Register New Subject (UC-05)");
    availableSubjectsTable = new QTableWidget(0, 2);
    availableSubjectsTable->setHorizontalHeaderLabels({"Subject", "Credits"});
    availableSubjectsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    btnRegisterSubject = new QPushButton("Register Selected Subject");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    btnChangePassStudent = new QPushButton("Change Password");
    btnLogoutStudent = new QPushButton("Logout");
    bottomLayout->addWidget(btnChangePassStudent);
    bottomLayout->addWidget(btnLogoutStudent);

    layout->addWidget(lbl);
    layout->addWidget(lblWelcomeStudent);
    layout->addWidget(lblStudentInfo);
    layout->addWidget(lblGPA);
    layout->addWidget(studentTable);
    layout->addWidget(btnDropSubject);
    layout->addWidget(lblAvailable);
    layout->addWidget(availableSubjectsTable);
    layout->addWidget(btnRegisterSubject);
    layout->addLayout(bottomLayout);

    connect(btnLogoutStudent, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(btnChangePassStudent, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);
    connect(btnDropSubject, &QPushButton::clicked, this, &MainWindow::onDropSubjectClicked);
    connect(btnRegisterSubject, &QPushButton::clicked, this, &MainWindow::onRegisterSubjectClicked);
}

void MainWindow::onLogoutClicked() {
    controller.logout();
    currentUsername.clear();
    currentUser.reset();
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::onSearchClicked() {
    std::string keyword = txtSearch->text().trimmed().toStdString();
    std::vector<Student> students = keyword.empty()
                                        ? controller.getAllStudents()
                                        : controller.searchStudents(keyword);

    adminTable->setRowCount(0);
    int row = 0;
    for (size_t i = 0; i < students.size(); ++i) {
        adminTable->insertRow(row);
        adminTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(students[i].getId())));
        adminTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(students[i].getName())));
        adminTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(students[i].getEmail())));
        adminTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(students[i].getPhone())));
        adminTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(students[i].getClassCode())));
        adminTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(students[i].getDateOfBirth())));
        adminTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(students[i].getGender())));
        adminTable->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(students[i].getAddress())));
        row++;
    }
}

void MainWindow::onAddStudentClicked() {
    std::string err;
    bool success = controller.addStudent(
        txtId->text().toStdString(),
        txtName->text().toStdString(),
        txtEmail->text().toStdString(),
        txtPhone->text().toStdString(),
        txtClass->text().toStdString(),
        txtDob->text().toStdString(),
        cmbGender->currentText().toStdString(),
        txtAddress->text().toStdString(),
        err
        );

    if (success) {
        QMessageBox::information(this, "Success", "Student added successfully!");
        onSearchClicked();
        txtId->clear(); txtName->clear(); txtEmail->clear(); txtPhone->clear();
        txtClass->clear(); txtDob->clear(); txtAddress->clear(); cmbGender->setCurrentIndex(0);
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::onUpdateStudentClicked() {
    std::string err;
    bool success = controller.updateStudent(
        txtId->text().toStdString(),
        txtName->text().toStdString(),
        txtEmail->text().toStdString(),
        txtPhone->text().toStdString(),
        txtClass->text().toStdString(),
        txtDob->text().toStdString(),
        cmbGender->currentText().toStdString(),
        txtAddress->text().toStdString(),
        err
        );

    if (success) {
        QMessageBox::information(this, "Success", "Student updated successfully!");
        onSearchClicked();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::onDeleteStudentClicked() {
    QString id = txtId->text();
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select or enter Student ID to delete!");
        return;
    }

    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm", "Are you sure you want to delete student " + id + "?",
        QMessageBox::Yes | QMessageBox::No);
    if (confirm != QMessageBox::Yes) return;

    if (controller.deleteStudent(id.toStdString())) {
        QMessageBox::information(this, "Success", "Student deleted successfully!");
        onSearchClicked();
        txtId->clear(); txtName->clear(); txtEmail->clear(); txtPhone->clear();
        txtClass->clear(); txtDob->clear(); txtAddress->clear(); cmbGender->setCurrentIndex(0);
    } else {
        QMessageBox::warning(this, "Error", "Student not found for deletion!");
    }
}

void MainWindow::onTableClicked(int row, int col) {
    Q_UNUSED(col);
    txtId->setText(adminTable->item(row, 0)->text());
    txtName->setText(adminTable->item(row, 1)->text());
    txtEmail->setText(adminTable->item(row, 2)->text());
    txtPhone->setText(adminTable->item(row, 3)->text());
    txtClass->setText(adminTable->item(row, 4)->text());
    txtDob->setText(adminTable->item(row, 5)->text());
    int genderIndex = cmbGender->findText(adminTable->item(row, 6)->text());
    cmbGender->setCurrentIndex(genderIndex >= 0 ? genderIndex : 0);
    txtAddress->setText(adminTable->item(row, 7)->text());
}

void MainWindow::refreshTeacherView() {
    std::vector<GradeRow> rows = controller.getTeacherGradebook(currentUsername);

    teacherTable->setRowCount(0);
    for (size_t i = 0; i < rows.size(); ++i) {
        int row = teacherTable->rowCount();
        teacherTable->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::fromStdString(rows[i].studentId));
        idItem->setData(Qt::UserRole, rows[i].enrollmentId);

        QString scoreText = rows[i].score < 0 ? "Ungraded" : QString::number(rows[i].score, 'f', 1);

        teacherTable->setItem(row, 0, idItem);
        teacherTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rows[i].studentName)));
        teacherTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(rows[i].subjectName)));
        teacherTable->setItem(row, 3, new QTableWidgetItem(scoreText));
    }

    selectedEnrollmentId = -1;
    lblTeacherSelection->setText(rows.empty()
                                     ? "You are not assigned to any class/subject (see Manage Teaching Assignments)."
                                     : "Select a row to grade");
}

void MainWindow::onTeacherRowClicked(int row, int col) {
    Q_UNUSED(col);
    QTableWidgetItem *idItem = teacherTable->item(row, 0);
    if (!idItem) return;

    selectedEnrollmentId = idItem->data(Qt::UserRole).toInt();
    QString studentName = teacherTable->item(row, 1)->text();
    QString subjectName = teacherTable->item(row, 2)->text();
    lblTeacherSelection->setText("Grading: " + studentName + " - " + subjectName);

    QTableWidgetItem *scoreItem = teacherTable->item(row, 3);
    QString scoreText = scoreItem->text();
    txtTeacherScore->setText(scoreText == "Ungraded" ? "" : scoreText);
}

void MainWindow::onSaveGradeClicked() {
    if (selectedEnrollmentId < 0) {
        QMessageBox::warning(this, "Error", "Please select an enrolled student to grade!");
        return;
    }

    bool ok = false;
    double score = txtTeacherScore->text().replace(',', '.').toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Error", "Score must be a valid number!");
        return;
    }

    std::string err;
    if (controller.updateGrade(selectedEnrollmentId, score, err)) {
        QMessageBox::information(this, "Success", "Grade saved successfully!");
        refreshTeacherView();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::refreshStudentView() {
    StudentProfile profile = controller.getStudentProfile(currentUsername);

    if (!profile.found) {
        lblStudentInfo->setText("Student profile not found for account: " + QString::fromStdString(currentUsername));
        lblGPA->setText("");
        studentTable->setRowCount(0);
        availableSubjectsTable->setRowCount(0);
        return;
    }

    lblStudentInfo->setText(QString("Full Name: %1 | Student ID: %2 | Class: %3")
                                .arg(QString::fromStdString(profile.name))
                                .arg(QString::fromStdString(profile.id))
                                .arg(QString::fromStdString(profile.classCode)));

    std::vector<SubjectResult> results = controller.getStudentResults(currentUsername);
    double gpa = StudentController::computeGPA(results);

    QString rank;
    if (gpa < 0) rank = "Ungraded";
    else if (gpa >= 8.5) rank = "Excellent";
    else if (gpa >= 7.0) rank = "Good";
    else if (gpa >= 5.5) rank = "Fair";
    else if (gpa >= 4.0) rank = "Average";
    else rank = "Poor";

    lblGPA->setText(gpa < 0
                        ? "Cumulative GPA: No data"
                        : QString("Cumulative GPA: %1 - %2").arg(gpa, 0, 'f', 2).arg(rank));

    studentTable->setRowCount(0);
    for (size_t i = 0; i < results.size(); ++i) {
        int row = studentTable->rowCount();
        studentTable->insertRow(row);
        QTableWidgetItem *subjectItem = new QTableWidgetItem(QString::fromStdString(results[i].subjectName));
        subjectItem->setData(Qt::UserRole, results[i].enrollmentId);
        studentTable->setItem(row, 0, subjectItem);
        studentTable->setItem(row, 1, new QTableWidgetItem(QString::number(results[i].credits)));
        QString scoreText = results[i].score < 0 ? "Ungraded" : QString::number(results[i].score, 'f', 1);
        studentTable->setItem(row, 2, new QTableWidgetItem(scoreText));
    }

    std::vector<SubjectInfo> available = controller.getAvailableSubjects(currentUsername);
    availableSubjectsTable->setRowCount(0);
    for (size_t i = 0; i < available.size(); ++i) {
        int row = availableSubjectsTable->rowCount();
        availableSubjectsTable->insertRow(row);
        QTableWidgetItem *subjectItem = new QTableWidgetItem(QString::fromStdString(available[i].name));
        subjectItem->setData(Qt::UserRole, QString::fromStdString(available[i].code));
        availableSubjectsTable->setItem(row, 0, subjectItem);
        availableSubjectsTable->setItem(row, 1, new QTableWidgetItem(QString::number(available[i].credits)));
    }
}

void MainWindow::onRegisterSubjectClicked() {
    int row = availableSubjectsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a subject to register!");
        return;
    }
    std::string subjectCode = availableSubjectsTable->item(row, 0)->data(Qt::UserRole).toString().toStdString();

    std::string err;
    if (controller.enrollStudent(currentUsername, subjectCode, err)) {
        QMessageBox::information(this, "Success", "Subject registered successfully!");
        refreshStudentView();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::onDropSubjectClicked() {
    int row = studentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a subject to drop!");
        return;
    }
    int enrollmentId = studentTable->item(row, 0)->data(Qt::UserRole).toInt();

    std::string err;
    if (controller.dropEnrollment(enrollmentId, err)) {
        QMessageBox::information(this, "Success", "Subject dropped successfully!");
        refreshStudentView();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::onChangePasswordClicked() {
    bool ok = false;
    QString oldPass = QInputDialog::getText(this, "Change Password", "Current password:",
                                            QLineEdit::Password, "", &ok);
    if (!ok) return;

    QString newPass = QInputDialog::getText(this, "Change Password", "New password:",
                                            QLineEdit::Password, "", &ok);
    if (!ok) return;

    QString confirmPass = QInputDialog::getText(this, "Change Password", "Confirm new password:",
                                                QLineEdit::Password, "", &ok);
    if (!ok) return;

    std::string err;
    if (controller.changePassword(currentUsername, oldPass.toStdString(), newPass.toStdString(),
                                  confirmPass.toStdString(), err)) {
        QMessageBox::information(this, "Success", "Password changed successfully!");
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::refreshClasses() {
    std::vector<ClassInfo> classes = controller.getAllClasses();
    classesTable->setRowCount(0);
    for (size_t i = 0; i < classes.size(); ++i) {
        int row = classesTable->rowCount();
        classesTable->insertRow(row);
        classesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(classes[i].code)));
        classesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(classes[i].name)));
    }
}

void MainWindow::refreshSubjects() {
    std::vector<SubjectInfo> subjects = controller.getAllSubjects();
    subjectsTable->setRowCount(0);
    for (size_t i = 0; i < subjects.size(); ++i) {
        int row = subjectsTable->rowCount();
        subjectsTable->insertRow(row);
        subjectsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(subjects[i].code)));
        subjectsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(subjects[i].name)));
        subjectsTable->setItem(row, 2, new QTableWidgetItem(QString::number(subjects[i].credits)));
    }
}

void MainWindow::onAddClassClicked() {
    std::string err;
    if (controller.addClass(txtClassCode->text().toStdString(), txtClassName->text().toStdString(), err)) {
        QMessageBox::information(this, "Success", "Class added successfully!");
        txtClassCode->clear(); txtClassName->clear();
        refreshClasses();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::onAddSubjectClicked() {
    bool ok = false;
    int credits = txtCredits->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Error", "Credits must be an integer!");
        return;
    }

    std::string err;
    if (controller.addSubject(txtSubjectCode->text().toStdString(), txtSubjectName->text().toStdString(), credits, err)) {
        QMessageBox::information(this, "Success", "Subject added successfully to database.");
        txtSubjectCode->clear(); txtSubjectName->clear(); txtCredits->clear();
        refreshSubjects();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::refreshEnrollments() {
    std::vector<EnrollmentInfo> rows = controller.getAllEnrollments();
    enrollmentsTable->setRowCount(0);
    for (size_t i = 0; i < rows.size(); ++i) {
        int row = enrollmentsTable->rowCount();
        enrollmentsTable->insertRow(row);
        enrollmentsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(rows[i].studentId)));
        enrollmentsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rows[i].studentName)));
        enrollmentsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(rows[i].subjectCode)));
        enrollmentsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(rows[i].subjectName)));
        QString scoreText = rows[i].score < 0 ? "Ungraded" : QString::number(rows[i].score, 'f', 1);
        enrollmentsTable->setItem(row, 4, new QTableWidgetItem(scoreText));
    }
}

void MainWindow::onEnrollClicked() {
    std::string err;
    if (controller.enrollStudent(txtEnrollStudentId->text().toStdString(),
                                 txtEnrollSubjectCode->text().toStdString(), err)) {
        QMessageBox::information(this, "Success", "Enrollment record created successfully in database.");
        txtEnrollStudentId->clear(); txtEnrollSubjectCode->clear();
        refreshEnrollments();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::refreshAccounts() {
    std::vector<std::pair<std::string, std::string>> accounts = controller.getAllAccounts();
    accountsTable->setRowCount(0);
    for (size_t i = 0; i < accounts.size(); ++i) {
        int row = accountsTable->rowCount();
        accountsTable->insertRow(row);
        accountsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(accounts[i].first)));
        accountsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(accounts[i].second)));
    }
}

void MainWindow::onAddAccountClicked() {
    std::string err;
    if (controller.addAccount(txtAccUsername->text().toStdString(), txtAccPassword->text().toStdString(),
                              cmbAccRole->currentText().toStdString(), err)) {
        QMessageBox::information(this, "Success", "Account created successfully!");
        txtAccUsername->clear(); txtAccPassword->clear(); cmbAccRole->setCurrentIndex(0);
        refreshAccounts();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::refreshAssignments() {
    std::vector<AssignmentInfo> rows = controller.getAllAssignments();
    assignmentsTable->setRowCount(0);
    for (size_t i = 0; i < rows.size(); ++i) {
        int row = assignmentsTable->rowCount();
        assignmentsTable->insertRow(row);
        assignmentsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(rows[i].teacherName)));
        assignmentsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rows[i].classCode)));
        assignmentsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(rows[i].subjectCode)));
    }
}

void MainWindow::onAssignTeacherClicked() {
    std::string err;
    if (controller.assignTeacher(txtAssignTeacher->text().toStdString(), txtAssignClass->text().toStdString(),
                                 txtAssignSubject->text().toStdString(), err)) {
        QMessageBox::information(this, "Success", "Teaching assignment successful!");
        txtAssignTeacher->clear(); txtAssignClass->clear(); txtAssignSubject->clear();
        refreshAssignments();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(err));
    }
}

void MainWindow::refreshAdminAll() {
    onSearchClicked();
    refreshClasses();
    refreshSubjects();
    refreshEnrollments();
    refreshAssignments();
    refreshAccounts();
}