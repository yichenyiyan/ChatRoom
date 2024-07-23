#include "login.h"


Login::Login(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(400, 600);
    centerWindow();
    this->setStyleSheet("background-color: lightblue;");
    QVBoxLayout *layout = new QVBoxLayout(this);

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("username");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("登录", this);
    backButton = new QPushButton("返回", this);

    loginButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");
    backButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");

    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
    layout->addWidget(backButton);

    connect(loginButton, &QPushButton::clicked, this, &Login::on_loginButton_clicked);
    connect(backButton, &QPushButton::clicked, this, &Login::on_backButton_clicked);

}

Login::~Login() {}

void Login::on_loginButton_clicked() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString command = "2 " + username + " " + password;

//    tsocket->sendCommand(command);
//    QObject::connect(tsocket, &TcpClient::receivedMessage, [&](){
//        QString msg = tsocket->getMessage();
//        if (msg == "2") {
//            CustomMessageBox msgBox(this);
//            msgBox.setText("登录成功！");
//            msgBox.setStandardButtons(QMessageBox::Ok);
//            if (msgBox.exec() == QMessageBox::Ok) {
//                Menu* menu = new Menu(username);
//                menu->show();

//                this->win->close();
//                this->close();
//            }
//        } else {
//            QString reason;
//            if (msg == "3") {
//                reason = "密码错误";
//            } else if (msg == "1") {
//                reason = "该用户已在别处登录";
//            } else if (msg == "0") {
//                reason = "该用户未注册";
//            }
//            CustomMessageBox msgBox(this);
//            msgBox.setText("登录失败！\n" + reason);
//            msgBox.setStandardButtons(QMessageBox::Ok);
//            if (msgBox.exec() == QMessageBox::Ok) {
//                this->win->show();
//                this->close();
//            }
//        }
//    });
}

void Login::on_backButton_clicked() {
    this->close();
}


void Login::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}


void Login::closeEvent(QCloseEvent *event) {
    event->accept();
}
