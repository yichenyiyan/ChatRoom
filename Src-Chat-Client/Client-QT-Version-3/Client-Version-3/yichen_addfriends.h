#ifndef YICHEN_ADDFRIENDS_H
#define YICHEN_ADDFRIENDS_H

#include <QDialog>
#include <QPushButton>
#include <QTcpSocket>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHostAddress>


class yichen_addFriends : public QDialog {
    Q_OBJECT
public:
    yichen_addFriends(QWidget* parent = 0);
    ~yichen_addFriends();

private:
    QGridLayout *mainLayout;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QPushButton *enterBtn;

    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;

public slots:
    void slotEnter();
    void dataReceived();

};

#endif // YICHEN_ADDFRIENDS_H
