#include "registerwindow.h"
#include "loginwindow.h"
#include "tcpclient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QScreen>
#include <QApplication>
#include <QMessageBox>

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("注册");
    setFixedSize(400, 600);
    centerWindow();
    setStyleSheet("background-color: lightblue;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("用户名");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("密码");
    passwordInput->setEchoMode(QLineEdit::Password);

    togglePasswordButton = new QPushButton(this);
    togglePasswordButton->setIcon(style()->standardIcon(QStyle::SP_DesktopIcon));
    togglePasswordButton->setFixedSize(passwordInput->height() - 2, passwordInput->height() - 2);
    togglePasswordButton->setCheckable(true);
    togglePasswordButton->setFlat(true);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordInput);
    passwordLayout->addWidget(togglePasswordButton);
    passwordLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(usernameInput);

    tcpclient = new TcpClient();
    QPushButton *registerButton = new QPushButton("注册", this);
    QPushButton *backButton = new QPushButton("返回", this);

    QString buttonStyle = "QPushButton { border: 2px solid #8f8f91; border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); }";

    registerButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet(buttonStyle);

    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(backButton);

    connect(togglePasswordButton, &QPushButton::toggled, this, &RegisterWindow::togglePasswordVisibility);
    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::registerUser);
    connect(backButton, &QPushButton::clicked, [&](){
            this->deleteLater();
            LoginWindow* loginWindow = new LoginWindow();
            loginWindow->show();
        });

}

void RegisterWindow::togglePasswordVisibility() {
    if (togglePasswordButton->isChecked()) {
        passwordInput->setEchoMode(QLineEdit::Normal);
    } else {
        passwordInput->setEchoMode(QLineEdit::Password);
    }
}

void RegisterWindow::registerUser() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空！");
        return;
    }

    tcpclient->sendCommand("1 "+username + " " + password);
    connect(tcpclient, &TcpClient::receivedMessage, [&](){
        QString reply = tcpclient->getMessage();
        if ( reply == "0") {
            QMessageBox::information(this, "成功", "注册成功!");
        } else if (reply == "1") {
            QMessageBox::information(this, "失败", "注册失败!");
        } else {
            QMessageBox::information(this, "error", "server error!");
        }
        this->deleteLater();

        LoginWindow *loginWindow = new LoginWindow();
        loginWindow->show();
    });

}

void RegisterWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

void RegisterWindow::closeEvent(QCloseEvent *event) {
    this->deleteLater();
    LoginWindow *loginWindow = new LoginWindow();
    loginWindow->show();
    event->accept();
}

RegisterWindow::~RegisterWindow() {
    delete tcpclient;
}
