#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <memory>
#include "StudentController.h"
#include "User.h"

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
    std::unique_ptr<User> currentUser;

    QStackedWidget *stackedWidget;
    QWidget *loginWidget;
    QWidget *adminWidget;
    QWidget *teacherWidget;
    QWidget *studentWidget;

    QLineEdit *txtUser, *txtPass;
    QPushButton *btnLogin;

    QTabWidget *adminTabs;
    QLabel *lblWelcomeAdmin;
    QLineEdit *txtId, *txtName, *txtEmail, *txtPhone, *txtClass, *txtSearch, *txtDob, *txtAddress;
    QComboBox *cmbGender;
    QPushButton *btnAdd, *btnUpdate, *btnDelete, *btnSearch, *btnLogoutAdmin, *btnChangePassAdmin;
    QTableWidget *adminTable;

    QLineEdit *txtClassCode, *txtClassName;
    QPushButton *btnAddClass;
    QTableWidget *classesTable;
    QLineEdit *txtSubjectCode, *txtSubjectName, *txtCredits;
    QPushButton *btnAddSubject;
    QTableWidget *subjectsTable;

    QLineEdit *txtEnrollStudentId, *txtEnrollSubjectCode;
    QPushButton *btnEnroll;
    QTableWidget *enrollmentsTable;

    QLineEdit *txtAccUsername, *txtAccPassword;
    QComboBox *cmbAccRole;
    QPushButton *btnAddAccount;
    QTableWidget *accountsTable;
    QLineEdit *txtAssignTeacher, *txtAssignClass, *txtAssignSubject;
    QPushButton *btnAssignTeacher;
    QTableWidget *assignmentsTable;

    QTableWidget *teacherTable;
    QLabel *lblWelcomeTeacher;
    QLineEdit *txtTeacherScore;
    QLabel *lblTeacherSelection;
    QPushButton *btnSaveGrade, *btnLogoutTeacher, *btnChangePassTeacher;
    int selectedEnrollmentId = -1;

    QLabel *lblWelcomeStudent, *lblStudentInfo, *lblGPA;
    QTableWidget *studentTable;
    QPushButton *btnDropSubject;
    QTableWidget *availableSubjectsTable;
    QPushButton *btnRegisterSubject;
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

    void refreshClasses();
    void refreshSubjects();
    void refreshEnrollments();
    void refreshAssignments();
    void refreshAccounts();
    void refreshAdminAll();

    void onAddClassClicked();
    void onAddSubjectClicked();
    void onEnrollClicked();
    void onAddAccountClicked();
    void onAssignTeacherClicked();

    void onRegisterSubjectClicked();
    void onDropSubjectClicked();
};

#endif