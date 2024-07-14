#ifndef YICHEN_TCP_H
#define YICHEN_TCP_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QHostAddress>
#include <QTcpSocket>

class yichen_tcp : public QDialog
{
    Q_OBJECT
public:
    yichen_tcp(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~yichen_tcp();
    QString username;

private:
    QListWidget *contentListWidget;
    QLineEdit *sendLineEdit;
    QPushButton *sendBtn;
    QPushButton *clearBtn;
    QGridLayout *mainLayout;
    int port;
    QString IP;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;

public slots:
    void slotConnected();
    void dataReceived();
    void slotSend();
    void slotClear();
};

#endif // YICHEN_TCP_H
