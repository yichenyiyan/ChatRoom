#ifndef FRIENDLISTWINDOW_H
#define FRIENDLISTWINDOW_H

#include <QScreen>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCloseEvent>

class TcpClient;
class MessageListWindow;

class FriendListWindow : public QMainWindow {
    Q_OBJECT

public:
    FriendListWindow(const QString& _username, MessageListWindow* _msglist, QWidget *parent = nullptr);
    ~FriendListWindow();

private:
    void centerWindow();

signals:
    void newMessageWin(const QString &username, const QString &message);
    void friendSelected(const QString& friendName);
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    //void weChat(const QString& _friendName);
    void onFriendClicked(QListWidgetItem *item);

private:
    MessageListWindow* msglist;
    QString username;
    TcpClient* tsocket;
    QListWidget *friendListWidget;
};

#endif // FRIENDLISTWINDOW_H
