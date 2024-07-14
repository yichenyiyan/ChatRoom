#ifndef YICHEN_DIALOG_H
#define YICHEN_DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTcpSocket>
#include <QListWidget>
#include <QGridLayout>
#include <QHostAddress>


class yichen_dialog : public QDialog
{
    Q_OBJECT

public:
    yichen_dialog(QWidget *parent = nullptr);
    ~yichen_dialog();

private:
    QGridLayout* mainLayout;
    QListWidget* contentListWidget;
    QPushButton* signINBtn;
    QPushButton* signUPBtn;
    QPushButton* disconnectBtn;

    int Server_port;
    QHostAddress* Server_ip;
    QTcpSocket* TcpSocket;

public slots:
    void slot_Signin();
    void slot_Signup();
    void slot_Disconnect();
    void data_Received();


};
#endif // YICHEN_DIALOG_H
