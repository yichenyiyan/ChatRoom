#include "yichen_dialog.h"

// yichen chat client version 1.0

yichen_dialog::yichen_dialog(QWidget *parent)
    : QDialog(parent), Server_port(9999)
{
    setWindowTitle(tr("MyChat"));
    this->resize(400, 400);
    contentListWidget = new QListWidget;
    signINBtn = new QPushButton(tr("Sign in"));
    signUPBtn = new QPushButton(tr("Sign up"));
    disconnectBtn = new QPushButton(tr("Disconnect"));
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(contentListWidget, 0, 0, 1, 3);
    mainLayout->addWidget(signINBtn, 1, 0);
    mainLayout->addWidget(signUPBtn, 1, 1);
    mainLayout->addWidget(disconnectBtn, 1, 2);

    connect(signINBtn, SIGNAL(clicked()), this, SLOT(slot_Signin()));
    connect(signUPBtn, SIGNAL(clicked()), this, SLOT(slot_Signup()));

    /*connect server*/
    TcpSocket = new QTcpSocket(this);
    connect(disconnectBtn, SIGNAL(clicked()), this, SLOT(slot_Disconnect()));
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(data_Received()));
    Server_ip = new QHostAddress();
    QString ip = "0.0.0.0";
    Server_ip->setAddress(ip);
    TcpSocket->connectToHost(*Server_ip, Server_port);
}

yichen_dialog::~yichen_dialog() {
#if 0
    delete TcpSocket;
    delete Server_ip;
    delete contentListWidget;
    delete signINBtn;
    delete signUPBtn;
    delete disconnectBtn;
#endif
}



void
yichen_dialog::slot_Signin() {
    QString msg = tr("Sign in");
    int length = TcpSocket->write(msg.toLatin1(), msg.length());
    if (length != msg.length()) return;
}


void
yichen_dialog::slot_Signup() {
    QString msg = tr("Sign up");
    int length = TcpSocket->write(msg.toLatin1(), msg.length());
    if (length != msg.length()) return;
}



void
yichen_dialog::slot_Disconnect() {
    TcpSocket->disconnectFromHost();
}



void
yichen_dialog::data_Received() {
    QByteArray datagram;
    datagram.resize(TcpSocket->bytesAvailable());
    TcpSocket->read(datagram.data(), datagram.size());
    QString msg = datagram.data();
    contentListWidget->addItem(msg.left(datagram.size()));
    // automatic infomation back
    contentListWidget->scrollToBottom();
}














