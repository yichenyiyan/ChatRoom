#ifndef MENU_H
#define MENU_H

#include <QLabel>
#include <QWidget>
#include <QApplication>
#include <QScreen>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QListWidgetItem>

class TcpClient;
class MessageListWindow;

class Menu : public QWidget {
    Q_OBJECT

public:
    explicit Menu(const QString& _user, QWidget *parent = nullptr);
    ~Menu();


private slots:
    void showMessagesPage();
    void showFunctionsPage();
    void onFriendChatButtonClicked();
    void onGroupChatButtonClicked();
    void onAddFriendButtonClicked();
    void onAddGroupButtonClicked();
    void onCreateGroupButtonClicked();

private:
    void setupUI();
    void centerWindow();
    void setupConnections();

    QPushButton *messageButton;
    QPushButton *functionButton;
    MessageListWindow *messagesPage;
    QWidget *functionsPage;
    QStackedWidget *stackedWidget;
    QVBoxLayout *functionsLayout;
    QListWidget *messageListWidget;
    QVBoxLayout *mainLayout;

    QString user;

    TcpClient *tcpclient;

};

#endif // MENU_H
