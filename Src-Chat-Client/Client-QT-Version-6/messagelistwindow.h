#ifndef MESSAGELISTWINDOW_H
#define MESSAGELISTWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QMap>
#include <QScrollArea>
#include "tcpclientthread.h"
#include "messagedetailwindow.h"

class MessageListWindow : public QWidget {
    Q_OBJECT

public:
    explicit MessageListWindow(const QString& _user, QWidget *parent = nullptr);
    ~MessageListWindow();
    void toAddMessage(const QString &username, const QString &message);
    void toOpenMessageDetailWindow(const QString &username);
    void toAddGMessage(const QString &username, const QString &message);
    void toOpenGMessageDetailWindow(const QString &username);

public slots:
    void addGroupWin(const QString& req_user, const QString& groupname);
    void addFriendWin(const QString& req_user, const QString& tar_user);
    void addMessage(const QString &username, const QString &message);
    void addGMessage(const QString &username, const QString &message);
    void openMessageDetailWindow(const QString &username);
    void openGMessageDetailWindow(const QString &username);

private:
    void setupUI();

    QString user;
    QVBoxLayout *mainLayout;
    QVBoxLayout *containerLayout;
    QMap<QString, QPushButton*> userButtons;
    QMap<QString, MessageDetailWindow*> userWindows;
    TcpClientThread *tcpClientThread;
    QWidget *container;
    QScrollArea *scrollArea;
};

#endif // MESSAGELISTWINDOW_H
