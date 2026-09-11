#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    refreshTable();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    this->setWindowTitle("Student Management System - Group 06");
    this->resize(900, 600);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // ĐĂNG NHẬP
    loginWidget = new QWidget();
    QVBoxLayout *loginLayout = new QVBoxLayout(loginWidget);
    txtUser = new QLineEdit(); txtUser->setPlaceholderText("Username (admin)");
    txtPass = new QLineEdit(); txtPass->setPlaceholderText("Password (Admin@123)");
    txtPass->setEchoMode(QLineEdit::Password);
    btnLogin = new QPushButton("Đăng Nhập");

    QLabel *title = new QLabel("<h2>HỆ THỐNG QUẢN LÝ SINH VIÊN</h2>");
    title->setAlignment(Qt::AlignCenter);

    loginLayout->addStretch();
    loginLayout->addWidget(title);
    loginLayout->addWidget(txtUser);
    loginLayout->addWidget(txtPass);
    loginLayout->addWidget(btnLogin);
    loginLayout->addStretch();

    // ADMIN MÀN HÌNH CHÍNH
    adminWidget = new QWidget();
    QVBoxLayout *adminLayout = new QVBoxLayout(adminWidget);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    txtId = new QLineEdit(); txtId->setPlaceholderText("Mã SV (ST000001)");
    txtName = new QLineEdit(); txtName->setPlaceholderText("Họ và Tên");
    txtPhone = new QLineEdit(); txtPhone->setPlaceholderText("SĐT (10 số)");
    txtClass = new QLineEdit(); txtClass->setPlaceholderText("Mã Lớp");
    inputLayout->addWidget(txtId); inputLayout->addWidget(txtName);
    inputLayout->addWidget(txtPhone); inputLayout->addWidget(txtClass);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Thêm");
    btnUpdate = new QPushButton("Cập Nhật");
    btnDelete = new QPushButton("Xóa");
    btnLayout->addWidget(btnAdd); btnLayout->addWidget(btnUpdate); btnLayout->addWidget(btnDelete);

    table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels({"Mã SV", "Họ Tên", "Số ĐT", "Lớp"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    btnChangePass = new QPushButton("Đổi Mật Khẩu");
    btnLogout = new QPushButton("Đăng Xuất");
    bottomLayout->addWidget(btnChangePass);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnLogout);

    adminLayout->addLayout(inputLayout);
    adminLayout->addLayout(btnLayout);
    adminLayout->addWidget(table);
    adminLayout->addLayout(bottomLayout);

    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(adminWidget);

    connect(btnLogin, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddStudentClicked);
    connect(btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateStudentClicked);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteStudentClicked);
    connect(btnChangePass, &QPushButton::clicked, this, &MainWindow::onChangePassClicked);
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(table, &QTableWidget::cellClicked, this, &MainWindow::onTableItemClicked);

    // --- BỘ TRANG TRÍ GIAO DIỆN QSS (Figma-style) ---
    this->setStyleSheet(R"(
        /* Màu nền tổng thể */
        QWidget {
            background-color: #1e1e2e;
            color: #cdd6f4;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }

        /* Tiêu đề */
        QLabel {
            color: #89b4fa;
            font-weight: bold;
        }

        /* Ô nhập liệu */
        QLineEdit {
            background-color: #313244;
            border: 1px solid #45475a;
            border-radius: 8px;
            padding: 10px;
            color: #cdd6f4;
        }
        QLineEdit:focus {
            border: 2px solid #89b4fa;
        }

        /* Nút bấm */
        QPushButton {
            background-color: #89b4fa;
            color: #11111b;
            border: none;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #b4befe;
        }
        QPushButton:pressed {
            background-color: #74c7ec;
        }

        /* Bảng dữ liệu */
        QTableWidget {
            background-color: #1e1e2e;
            border: 1px solid #45475a;
            border-radius: 8px;
            gridline-color: #313244;
            selection-background-color: #45475a;
            selection-color: #89b4fa;
        }
        QHeaderView::section {
            background-color: #313244;
            color: #cdd6f4;
            padding: 8px;
            border: none;
            font-weight: bold;
        }
    )");
}

void MainWindow::onTableItemClicked(int row, int col) {
    txtId->setText(table->item(row, 0)->text());
    txtName->setText(table->item(row, 1)->text());
    txtPhone->setText(table->item(row, 2)->text());
    txtClass->setText(table->item(row, 3)->text());
}

void MainWindow::onLoginClicked() {
    std::string user = txtUser->text().toStdString();
    std::string pass = txtPass->text().toStdString();
    if (controller.login(user, pass)) {
        stackedWidget->setCurrentWidget(adminWidget);
        txtUser->clear(); txtPass->clear();
    } else {
        QMessageBox::warning(this, "Lỗi", "Sai tài khoản hoặc mật khẩu!");
    }
}

void MainWindow::onAddStudentClicked() {
    std::string errorMsg;
    if (controller.addStudent(txtId->text().toStdString(), txtName->text().toStdString(),
                              txtPhone->text().toStdString(), txtClass->text().toStdString(), errorMsg)) {
        refreshTable();
        txtId->clear(); txtName->clear(); txtPhone->clear(); txtClass->clear();
        QMessageBox::information(this, "Thành công", "Đã thêm sinh viên!");
    } else { QMessageBox::warning(this, "Lỗi", QString::fromStdString(errorMsg)); }
}

void MainWindow::onUpdateStudentClicked() {
    std::string errorMsg;
    if (controller.updateStudent(txtId->text().toStdString(), txtPhone->text().toStdString(),
                                 txtClass->text().toStdString(), errorMsg)) {
        refreshTable();
        QMessageBox::information(this, "Thành công", "Cập nhật sinh viên thành công!");
    } else { QMessageBox::warning(this, "Lỗi", QString::fromStdString(errorMsg)); }
}

void MainWindow::onDeleteStudentClicked() {
    std::string id = txtId->text().toStdString();
    if (id.empty()) { QMessageBox::warning(this, "Lỗi", "Chọn sinh viên cần xóa!"); return; }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận", "Bạn có chắc chắn muốn xóa?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (controller.deleteStudent(id)) {
            refreshTable();
            txtId->clear(); txtName->clear(); txtPhone->clear(); txtClass->clear();
            QMessageBox::information(this, "Thành công", "Đã xóa sinh viên!");
        }
    }
}

void MainWindow::onChangePassClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Đổi Mật Khẩu");
    dialog.setStyleSheet(this->styleSheet()); // Áp dụng luôn giao diện đẹp cho hộp thoại
    QVBoxLayout form(&dialog);
    QLineEdit oldP, newP, confP;
    oldP.setPlaceholderText("Mật khẩu hiện tại"); oldP.setEchoMode(QLineEdit::Password);
    newP.setPlaceholderText("Mật khẩu mới"); newP.setEchoMode(QLineEdit::Password);
    confP.setPlaceholderText("Xác nhận MK mới"); confP.setEchoMode(QLineEdit::Password);
    QPushButton btnSave("Lưu");
    form.addWidget(&oldP); form.addWidget(&newP); form.addWidget(&confP); form.addWidget(&btnSave);

    connect(&btnSave, &QPushButton::clicked, [&]() {
        std::string errorMsg;
        if (controller.changePassword(oldP.text().toStdString(), newP.text().toStdString(), confP.text().toStdString(), errorMsg)) {
            QMessageBox::information(&dialog, "Thành công", "Đổi mật khẩu thành công!");
            dialog.accept();
        } else { QMessageBox::warning(&dialog, "Lỗi", QString::fromStdString(errorMsg)); }
    });
    dialog.exec();
}

void MainWindow::onLogoutClicked() { stackedWidget->setCurrentWidget(loginWidget); }

void MainWindow::refreshTable() {
    auto students = controller.getAllStudents();
    table->setRowCount(students.size());
    for (size_t i = 0; i < students.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(students[i].getId())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(students[i].getName())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(students[i].getPhone())));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(students[i].getClassCode())));
    }
}