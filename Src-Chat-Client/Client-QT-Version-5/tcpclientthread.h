#ifndef TCPCLIENTTHREAD_H
#define TCPCLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "tcpclient.h"

class TcpClientThread : public QThread {
    Q_OBJECT

public:
    explicit TcpClientThread(QObject *parent = nullptr);

    void run() override;
    void toUpdateServerFd(const QString& username);
    void toSend(const QString& username, const QString& target_user,const QString& msg);

signals:
    void newMessageReceived(const QString &username, const QString &message);
    void newMessageReceived2(const QString &req_user, const QString &tar_user);

private slots:
    void handleMessage();

private:
    TcpClient *tcpClient;
};

#endif // TCPCLIENTTHREAD_H
