#include "chat.h"
#include "tcpclient.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

Chat::Chat(QWidget *parent, TcpClient* _tsocket, QString src_user, QString target_user)
    : QWidget(parent), tsocket(_tsocket)
    , sender(new QString(src_user)), receiver(new QString(target_user)){
    this->setFixedSize(400, 400);
    QVBoxLayout *layout = new QVBoxLayout(this);

    if (!tsocket) {
        tsocket = new TcpClient(nullptr);
    }

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    messageLineEdit = new QLineEdit(this);
    sendMessageButton = new QPushButton("Send", this);

    layout->addWidget(chatDisplay);
    layout->addWidget(messageLineEdit);
    layout->addWidget(sendMessageButton);

    connect(sendMessageButton, &QPushButton::clicked, this, &Chat::on_sendMessageButton_clicked);
    qDebug() << "the Window was created! src_user is " << src_user << " target_user is " << target_user;
}

Chat::~Chat() {}

void Chat::on_sendMessageButton_clicked() {
    QString message = messageLineEdit->text();
    QString command = "5 " + *sender + " " + *receiver + " " + message;

    tsocket->sendCommand(command);
}
