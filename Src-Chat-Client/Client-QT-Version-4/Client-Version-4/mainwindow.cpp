#include "mainwindow.h"
#include "login.h"
#include "register.h"
#include <QScreen>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    this->setFixedSize(400, 600);
    centerWindow();
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    loginButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");
    registerButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");

    layout->addWidget(loginButton);
    layout->addWidget(registerButton);

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::on_loginButton_clicked);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::on_registerButton_clicked);

    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::on_loginButton_clicked() {
    Login *login = new Login(nullptr);
    connect(login, &Login::toBack, this, &MainWindow::showMainWindow);
    connect(login, &Login::destroyed, login, &QObject::deleteLater);
    login->show();
    this->hide();
}

void MainWindow::on_registerButton_clicked() {
    Register *reg = new Register(nullptr);
    connect(reg, &Register::toBack, this, &MainWindow::showMainWindow);
    connect(reg, &Register::destroyed, reg, &QObject::deleteLater);
    reg->show();
    this->hide();
}

void MainWindow::showMainWindow() {
    this->show();
}
