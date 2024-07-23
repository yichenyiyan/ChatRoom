#ifndef FRIENDLISTPAGE_H
#define FRIENDLISTPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "menu.h"
#include "tcpclient.h"

class FriendListPage : public QWidget {
    Q_OBJECT

public:
    FriendListPage(Menu* _menu, TcpClient* _tsocket, const QString& _user, QWidget *parent = nullptr);
    ~FriendListPage();

private slots:
    void onFriendClicked(QListWidgetItem *item);
    void loadFriendList();
private:
    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }
private:
    Menu* menu;
    TcpClient* tsocket;
    QString user;
    QVBoxLayout* layout;
    QListWidget* friendListWidget;
    QPushButton* backButton;
};

#endif // FRIENDLISTPAGE_H
