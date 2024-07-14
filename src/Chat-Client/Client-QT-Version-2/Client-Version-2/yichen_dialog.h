#ifndef YICHEN_DIALOG_H
#define YICHEN_DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QHostAddress>
#include <QTcpSocket>


class yichen_dialog : public QDialog
{
    Q_OBJECT

public:
    yichen_dialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~yichen_dialog();

private:
        QListWidget *contentListWidget;
        QLineEdit *sendLineEdit;
        QPushButton *sendBtn;
        QPushButton *clearBtn;
        QLabel *userNameLabel;
        QLineEdit *userNameLineEdit;
        QLabel *serverIPLabel;
        QLineEdit *serverIPLineEdit;
        QLabel *portLabel;
        QLineEdit *portLineEdit;
        QPushButton *enterBtn;
        QGridLayout *mainLayout;
        bool status;
        int port;
        QString IP;
        QHostAddress *serverIP;
        QString userName;
        QTcpSocket *tcpSocket;

public slots:
        void slotEnter();
        void slotConnected();
        void slotDisconnected();
        void dataReceived();
        void slotSend();
        void slotClear();

};
#endif // YICHEN_DIALOG_H
