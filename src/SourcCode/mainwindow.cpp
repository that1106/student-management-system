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
    QLabel *title = new QLabel("<h2>HE THONG QUAN LY SINH VIEN</h2>");
    title->setAlignment(Qt::AlignCenter);

    txtUser = new QLineEdit();
    txtUser->setPlaceholderText("Tai khoan (admin / teacher01 / ST000002)");

    txtPass = new QLineEdit();
    txtPass->setPlaceholderText("Mat khau");
    txtPass->setEchoMode(QLineEdit::Password);

    btnLogin = new QPushButton("DANG NHAP HE THONG");

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

    if (role == 1) {
        currentUsername = username;
        stackedWidget->setCurrentIndex(1);
        onSearchClicked();
    } else if (role == 2) {
        currentUsername = username;
        stackedWidget->setCurrentIndex(2);
        refreshTeacherView();
    } else if (role == 3) {
        currentUsername = username;
        stackedWidget->setCurrentIndex(3);
        refreshStudentView();
    } else {
        QMessageBox::warning(this, "Loi", "Tai khoan hoac mat khau khong chinh xac!");
    }
    txtUser->clear();
    txtPass->clear();
}

void MainWindow::setupAdminUI() {
    QVBoxLayout *layout = new QVBoxLayout(adminWidget);
    QLabel *lbl = new QLabel("<h3>QUAN TRI HE THONG - ADMIN WORKSPACE</h3>");

    // Form nhap lieu CRUD Sinh vien
    QGridLayout *formLayout = new QGridLayout();
    txtId = new QLineEdit(); txtId->setPlaceholderText("Ma SV (VD: ST000001)");
    txtName = new QLineEdit(); txtName->setPlaceholderText("Ho va Ten");
    txtEmail = new QLineEdit(); txtEmail->setPlaceholderText("Email");
    txtPhone = new QLineEdit(); txtPhone->setPlaceholderText("So dien thoai (10 so)");
    txtClass = new QLineEdit(); txtClass->setPlaceholderText("Ma lop (VD: ST101)");
    txtDob = new QLineEdit(); txtDob->setPlaceholderText("Ngay sinh (YYYY-MM-DD)");
    cmbGender = new QComboBox(); cmbGender->addItems({"Nam", "Nu", "Khac"});
    txtAddress = new QLineEdit(); txtAddress->setPlaceholderText("Dia chi");

    formLayout->addWidget(txtId, 0, 0);
    formLayout->addWidget(txtName, 0, 1);
    formLayout->addWidget(txtEmail, 0, 2);
    formLayout->addWidget(txtPhone, 1, 0);
    formLayout->addWidget(txtClass, 1, 1);
    formLayout->addWidget(txtDob, 1, 2);
    formLayout->addWidget(cmbGender, 2, 0);
    formLayout->addWidget(txtAddress, 2, 1, 1, 2);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Them");
    btnUpdate = new QPushButton("Cap Nhat");
    btnDelete = new QPushButton("Xoa");
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnUpdate);
    btnLayout->addWidget(btnDelete);

    // Tim kiem va Bang
    QHBoxLayout *searchLayout = new QHBoxLayout();
    txtSearch = new QLineEdit(); txtSearch->setPlaceholderText("Nhap Ma SV, Ten, Email, SDT hoac Lop de tim kiem...");
    btnSearch = new QPushButton("Tim kiem");
    searchLayout->addWidget(txtSearch);
    searchLayout->addWidget(btnSearch);

    adminTable = new QTableWidget(0, 8);
    adminTable->setHorizontalHeaderLabels({"Ma SV", "Ho Ten", "Email", "So DT", "Lop", "Ngay Sinh", "Gioi Tinh", "Dia Chi"});
    adminTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    btnChangePassAdmin = new QPushButton("Doi Mat Khau");
    btnLogoutAdmin = new QPushButton("Dang Xuat");
    bottomLayout->addWidget(btnChangePassAdmin);
    bottomLayout->addWidget(btnLogoutAdmin);

    layout->addWidget(lbl);
    layout->addLayout(formLayout);
    layout->addLayout(btnLayout);
    layout->addLayout(searchLayout);
    layout->addWidget(adminTable);
    layout->addLayout(bottomLayout);

    connect(btnLogoutAdmin, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(btnChangePassAdmin, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);
    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(txtSearch, &QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddStudentClicked);
    connect(btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateStudentClicked);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteStudentClicked);
    connect(adminTable, &QTableWidget::cellClicked, this, &MainWindow::onTableClicked);
}

void MainWindow::setupTeacherUI() {
    QVBoxLayout *layout = new QVBoxLayout(teacherWidget);
    QLabel *lbl = new QLabel("<h3>KHONG GIAN GIANG VIEN - TEACHER WORKSPACE</h3>");

    teacherTable = new QTableWidget(0, 4);
    teacherTable->setHorizontalHeaderLabels({"Ma SV", "Ho Ten", "Mon Hoc", "Diem Tong Ket"});
    teacherTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *gradeLayout = new QHBoxLayout();
    lblTeacherSelection = new QLabel("Chon mot dong de cham diem");
    txtTeacherScore = new QLineEdit(); txtTeacherScore->setPlaceholderText("Diem (0 - 10)");
    btnSaveGrade = new QPushButton("Luu Diem");
    gradeLayout->addWidget(lblTeacherSelection, 1);
    gradeLayout->addWidget(txtTeacherScore);
    gradeLayout->addWidget(btnSaveGrade);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    btnChangePassTeacher = new QPushButton("Doi Mat Khau");
    btnLogoutTeacher = new QPushButton("Dang Xuat");
    bottomLayout->addWidget(btnChangePassTeacher);
    bottomLayout->addWidget(btnLogoutTeacher);

    layout->addWidget(lbl);
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
    QLabel *lbl = new QLabel("<h3>KET QUA HOC TAP - STUDENT WORKSPACE</h3>");
    lblStudentInfo = new QLabel("...");
    lblGPA = new QLabel("...");

    studentTable = new QTableWidget(0, 3);
    studentTable->setHorizontalHeaderLabels({"Mon Hoc", "So Tin Chi", "Diem So"});
    studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    btnChangePassStudent = new QPushButton("Doi Mat Khau");
    btnLogoutStudent = new QPushButton("Dang Xuat");
    bottomLayout->addWidget(btnChangePassStudent);
    bottomLayout->addWidget(btnLogoutStudent);

    layout->addWidget(lbl);
    layout->addWidget(lblStudentInfo);
    layout->addWidget(lblGPA);
    layout->addWidget(studentTable);
    layout->addLayout(bottomLayout);

    connect(btnLogoutStudent, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(btnChangePassStudent, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);
}

void MainWindow::onLogoutClicked() {
    controller.logout();
    currentUsername.clear();
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
        QMessageBox::information(this, "Thanh cong", "Them sinh vien thanh cong!");
        onSearchClicked();
        txtId->clear(); txtName->clear(); txtEmail->clear(); txtPhone->clear();
        txtClass->clear(); txtDob->clear(); txtAddress->clear(); cmbGender->setCurrentIndex(0);
    } else {
        QMessageBox::warning(this, "Loi", QString::fromStdString(err));
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
        QMessageBox::information(this, "Thanh cong", "Cap nhat sinh vien thanh cong!");
        onSearchClicked();
    } else {
        QMessageBox::warning(this, "Loi", QString::fromStdString(err));
    }
}

void MainWindow::onDeleteStudentClicked() {
    QString id = txtId->text();
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Loi", "Vui long chon hoac nhap Ma SV can xoa!");
        return;
    }

    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Xac nhan", "Ban co chac muon xoa sinh vien " + id + " khong?",
        QMessageBox::Yes | QMessageBox::No);
    if (confirm != QMessageBox::Yes) return;

    if (controller.deleteStudent(id.toStdString())) {
        QMessageBox::information(this, "Thanh cong", "Da xoa sinh vien thanh cong!");
        onSearchClicked();
        txtId->clear(); txtName->clear(); txtEmail->clear(); txtPhone->clear();
        txtClass->clear(); txtDob->clear(); txtAddress->clear(); cmbGender->setCurrentIndex(0);
    } else {
        QMessageBox::warning(this, "Loi", "Khong tim thay sinh vien de xoa!");
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
        idItem->setData(Qt::UserRole, rows[i].enrollmentId); // hidden: which enrollment row to update

        QString scoreText = rows[i].score < 0 ? "Chua co diem" : QString::number(rows[i].score, 'f', 1);

        teacherTable->setItem(row, 0, idItem);
        teacherTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(rows[i].studentName)));
        teacherTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(rows[i].subjectName)));
        teacherTable->setItem(row, 3, new QTableWidgetItem(scoreText));
    }

    selectedEnrollmentId = -1;
    lblTeacherSelection->setText(rows.empty()
                                     ? "Ban chua duoc phan cong lop/mon hoc nao (xem Manage Teaching Assignments)."
                                     : "Chon mot dong de cham diem");
}

void MainWindow::onTeacherRowClicked(int row, int col) {
    Q_UNUSED(col);
    QTableWidgetItem *idItem = teacherTable->item(row, 0);
    if (!idItem) return;

    selectedEnrollmentId = idItem->data(Qt::UserRole).toInt();
    QString studentName = teacherTable->item(row, 1)->text();
    QString subjectName = teacherTable->item(row, 2)->text();
    lblTeacherSelection->setText("Dang cham: " + studentName + " - " + subjectName);

    QTableWidgetItem *scoreItem = teacherTable->item(row, 3);
    QString scoreText = scoreItem->text();
    txtTeacherScore->setText(scoreText == "Chua co diem" ? "" : scoreText);
}

void MainWindow::onSaveGradeClicked() {
    if (selectedEnrollmentId < 0) {
        QMessageBox::warning(this, "Loi", "Vui long chon mot sinh vien da dang ky mon hoc de cham diem!");
        return;
    }

    bool ok = false;
    double score = txtTeacherScore->text().replace(',', '.').toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Loi", "Diem so phai la mot so hop le!");
        return;
    }

    std::string err;
    if (controller.updateGrade(selectedEnrollmentId, score, err)) {
        QMessageBox::information(this, "Thanh cong", "Da luu diem thanh cong!");
        refreshTeacherView();
    } else {
        QMessageBox::warning(this, "Loi", QString::fromStdString(err));
    }
}


void MainWindow::refreshStudentView() {
    StudentProfile profile = controller.getStudentProfile(currentUsername);

    if (!profile.found) {
        lblStudentInfo->setText("Khong tim thay ho so sinh vien cho tai khoan: " + QString::fromStdString(currentUsername));
        lblGPA->setText("");
        studentTable->setRowCount(0);
        return;
    }

    lblStudentInfo->setText(QString("Ho ten: %1 | Ma SV: %2 | Lop: %3")
                                .arg(QString::fromStdString(profile.name))
                                .arg(QString::fromStdString(profile.id))
                                .arg(QString::fromStdString(profile.classCode)));

    std::vector<SubjectResult> results = controller.getStudentResults(currentUsername);
    double gpa = StudentController::computeGPA(results);

    QString rank;
    if (gpa < 0) rank = "Chua co diem";
    else if (gpa >= 8.5) rank = "Xuat sac";
    else if (gpa >= 7.0) rank = "Gioi";
    else if (gpa >= 5.5) rank = "Kha";
    else if (gpa >= 4.0) rank = "Trung binh";
    else rank = "Yeu";

    lblGPA->setText(gpa < 0
                        ? "Diem trung binh tich luy (GPA): chua co du lieu"
                        : QString("Diem trung binh tich luy (GPA): %1 - %2").arg(gpa, 0, 'f', 2).arg(rank));

    studentTable->setRowCount(0);
    for (size_t i = 0; i < results.size(); ++i) {
        int row = studentTable->rowCount();
        studentTable->insertRow(row);
        studentTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(results[i].subjectName)));
        studentTable->setItem(row, 1, new QTableWidgetItem(QString::number(results[i].credits)));
        QString scoreText = results[i].score < 0 ? "Chua co diem" : QString::number(results[i].score, 'f', 1);
        studentTable->setItem(row, 2, new QTableWidgetItem(scoreText));
    }
}


void MainWindow::onChangePasswordClicked() {
    bool ok = false;
    QString oldPass = QInputDialog::getText(this, "Doi Mat Khau", "Mat khau hien tai:",
                                            QLineEdit::Password, "", &ok);
    if (!ok) return;

    QString newPass = QInputDialog::getText(this, "Doi Mat Khau", "Mat khau moi:",
                                            QLineEdit::Password, "", &ok);
    if (!ok) return;

    QString confirmPass = QInputDialog::getText(this, "Doi Mat Khau", "Xac nhan mat khau moi:",
                                                QLineEdit::Password, "", &ok);
    if (!ok) return;

    std::string err;
    if (controller.changePassword(currentUsername, oldPass.toStdString(), newPass.toStdString(),
                                  confirmPass.toStdString(), err)) {
        QMessageBox::information(this, "Thanh cong", "Doi mat khau thanh cong!");
    } else {
        QMessageBox::warning(this, "Loi", QString::fromStdString(err));
    }
}