#ifndef PRIVATECHATPAGE_H
#define PRIVATECHATPAGE_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "friendlistpage.h"
#include "menu.h"
#include "tcpclient.h"

class PrivateChatPage : public QWidget {
    Q_OBJECT

public:
    PrivateChatPage(FriendListPage* _prevpage, Menu* _menu, TcpClient* _tsocket, const QString& username, const QString& friendName, QWidget *parent = nullptr);
    ~PrivateChatPage();

private slots:
    void onSendMessage();
    void onReceivedMessage();
private:
    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }
private:
    QString friendName;
    QString user;
    Menu* menu;
    TcpClient* tsocket;
    FriendListPage* prevpage;

    QVBoxLayout* layout;
    QTextEdit* chatDisplay;
    QLineEdit* messageInput;
    QPushButton* sendButton;
    QPushButton* backButton;
};

#endif // PRIVATECHATPAGE_H

