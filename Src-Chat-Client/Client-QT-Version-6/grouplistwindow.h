#ifndef GROUPLISTWINDOW_H
#define GROUPLISTWINDOW_H


#include <QScreen>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCloseEvent>

class TcpClient;
class MessageListWindow;

class GroupListWindow : public QMainWindow {
    Q_OBJECT

public:
    GroupListWindow(const QString& _username, MessageListWindow* _msglist, QWidget *parent = nullptr);
    ~GroupListWindow();

private:
    void centerWindow();

signals:
    void newMessageWin(const QString &groupName, const QString &message);
    void groupSelected(const QString& friendName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onFriendClicked(QListWidgetItem *item);

private:
    MessageListWindow* msglist;
    QString username;
    TcpClient* tsocket;
    QListWidget *friendListWidget;
};

#endif // GROUPLISTWINDOW_H
