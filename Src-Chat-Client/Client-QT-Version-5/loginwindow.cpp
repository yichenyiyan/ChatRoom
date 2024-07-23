#include "menu.h"
#include "loginwindow.h"
#include "registerwindow.h"
#include "tcpclient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QSpacerItem>
#include <QScreen>
#include <QMessageBox>
#include <QApplication>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("yichen-chatroom");
    setFixedSize(400, 600);
    centerWindow();
    setStyleSheet("background-color: lightblue;");
    QString buttonStyle = "QPushButton { border: 2px solid #8f8f91; border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); }";

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
    QPushButton *loginButton = new QPushButton("登录", this);
    QPushButton *registerButton = new QPushButton("注册", this);

    loginButton->setStyleSheet(buttonStyle);
    registerButton->setStyleSheet(buttonStyle);

    QHBoxLayout *registerLayout = new QHBoxLayout();
    registerLayout->addStretch();
    registerLayout->addWidget(registerButton);

    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addWidget(loginButton);
    mainLayout->addLayout(registerLayout);

    connect(togglePasswordButton, &QPushButton::toggled, this, &LoginWindow::togglePasswordVisibility);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegisterWindow);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::toVerifyLogin);
    connect(this, &LoginWindow::toShowMenu, this, &LoginWindow::ShowMenu);
    connect(tcpclient, &TcpClient::receivedMessage, [&](){
        QString reply = tcpclient->getMessage();
        if (reply == "0") {
            QMessageBox::information(this, "失败", "用户未注册!");
        } else if (reply == "1") {
            QMessageBox::information(this, "失败", "用户已在线!");
        } else if (reply == "2") {
            QMessageBox::information(this, "成功", "登录成功!");
            emit toShowMenu(usernameInput->text());
        } else if (reply == "3") {
            QMessageBox::information(this, "失败", "密码错误!");
        } else {
            QMessageBox::information(this, "error", "server error!");
        }
    });
}

void LoginWindow::togglePasswordVisibility() {
    if (togglePasswordButton->isChecked()) {
        passwordInput->setEchoMode(QLineEdit::Normal);
    } else {
        passwordInput->setEchoMode(QLineEdit::Password);
    }
}

void LoginWindow::toVerifyLogin() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空！");
        return;
    }

    tcpclient->sendCommand("2 "+ username + " " + password);

}

void LoginWindow::openRegisterWindow() {
    RegisterWindow *registerWindow = new RegisterWindow();
    registerWindow->show();
    close();
}

void LoginWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

LoginWindow::~LoginWindow() {
    qDebug() << "quit LoginWindow!";
    delete tcpclient;
}

void LoginWindow::ShowMenu(QString user) {
    Menu* menu = new Menu(user);
    menu->show();
    close();
}
