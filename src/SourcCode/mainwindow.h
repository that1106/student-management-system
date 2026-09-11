#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include "StudentController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onLoginClicked();
    void onAddStudentClicked();
    void onUpdateStudentClicked();
    void onDeleteStudentClicked();
    void onChangePassClicked();
    void onLogoutClicked();
    void onTableItemClicked(int row, int col);
private:
    StudentController controller;
    QStackedWidget *stackedWidget;

    QWidget *loginWidget;
    QLineEdit *txtUser;
    QLineEdit *txtPass;
    QPushButton *btnLogin;

    QWidget *adminWidget;
    QLineEdit *txtId;
    QLineEdit *txtName;
    QLineEdit *txtPhone;
    QLineEdit *txtClass;

    QPushButton *btnAdd;
    QPushButton *btnUpdate;
    QPushButton *btnDelete;
    QPushButton *btnChangePass;
    QPushButton *btnLogout;
    QTableWidget *table;

    void setupUI();
    void refreshTable();
};
#endif