#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include "StudentController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void setupLoginUI();
    void setupAdminUI();
    void setupTeacherUI();
    void setupStudentUI();

    StudentController controller;

    std::string currentUsername;

    QStackedWidget *stackedWidget;
    QWidget *loginWidget;
    QWidget *adminWidget;
    QWidget *teacherWidget;
    QWidget *studentWidget;

    // Login widgets
    QLineEdit *txtUser, *txtPass;
    QPushButton *btnLogin;

    // Admin widgets
    QLineEdit *txtId, *txtName, *txtEmail, *txtPhone, *txtClass, *txtSearch, *txtDob, *txtAddress;
    QComboBox *cmbGender;
    QPushButton *btnAdd, *btnUpdate, *btnDelete, *btnSearch, *btnLogoutAdmin, *btnChangePassAdmin;
    QTableWidget *adminTable;

    // Teacher widgets
    QTableWidget *teacherTable;
    QLineEdit *txtTeacherScore;
    QLabel *lblTeacherSelection;
    QPushButton *btnSaveGrade, *btnLogoutTeacher, *btnChangePassTeacher;
    int selectedEnrollmentId = -1;

    // Student widgets
    QLabel *lblStudentInfo, *lblGPA;
    QTableWidget *studentTable;
    QPushButton *btnLogoutStudent, *btnChangePassStudent;

private slots:
    void onLoginClicked();
    void onLogoutClicked();
    void onSearchClicked();
    void onAddStudentClicked();
    void onUpdateStudentClicked();
    void onDeleteStudentClicked();
    void onTableClicked(int row, int col);

    void refreshTeacherView();
    void onTeacherRowClicked(int row, int col);
    void onSaveGradeClicked();

    void refreshStudentView();

    void onChangePasswordClicked();
};

#endif // MAINWINDOW_H