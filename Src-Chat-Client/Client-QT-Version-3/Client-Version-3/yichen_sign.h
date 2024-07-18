#ifndef YICHEN_SIGN_H
#define YICHEN_SIGN_H


#include <QDialog>
#include <QPushButton>
#include <QTcpSocket>
#include <QGridLayout>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHostAddress>

#include "yichen_command.h"
#include "yichen_drawer.h"

class yichen_sign : public QDialog {
    Q_OBJECT

public:
    yichen_sign(QWidget* parent = 0);
    ~yichen_sign();

private:
    QListWidget *contentListWidget;
    QGridLayout *mainLayout;
    QLabel *nameLabel;
    QLabel *passwordLabel;
    QPushButton *connectBtn;
    QLineEdit *nameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *registerBtn;
    QPushButton *LogInBtn;

    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;

    QString username;

    yichen_drawer* drawer;

public slots:
    void slotRegister();
    void slotLogIn();
    void slotConnect();
    void dataReceived();
    void connected();
};

#endif // YICHEN_SIGN_H
