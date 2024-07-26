#include "entry.h"
#include "login.h"
#include <QDebug>

Entry::Entry(QWidget *parent)
    : QMainWindow(parent) {
    this->setFixedSize(400, 600);
    centerWindow();
    //setStyleSheet("background-color: lightblue;");
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    loginButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");
    registerButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");

    layout->addWidget(loginButton);
    layout->addWidget(registerButton);

    connect(loginButton, &QPushButton::clicked, this, &Entry::on_loginButton_clicked);
    connect(registerButton, &QPushButton::clicked, this, &Entry::on_registerButton_clicked);

    setCentralWidget(centralWidget);
}

Entry::~Entry() {}

void Entry::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

void Entry::closeEvent(QCloseEvent *event) {
    event->accept();
}

void Entry::on_loginButton_clicked() {
    Login* login = new Login(nullptr);
    login->show();
    this->hide();
}

void Entry::on_registerButton_clicked() {
//    Register *reg = new Register(this, nullptr);
//    reg->show();
//    this->hide();
}

void Entry::to_showMenu() {
    qDebug() << "to show Menu UI!";
    this->close();
}
