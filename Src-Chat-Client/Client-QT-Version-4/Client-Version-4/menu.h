#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QApplication>
#include <QScreen>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QListWidgetItem>

class TcpClient;

class Menu : public QWidget {
    Q_OBJECT

public:
    explicit Menu(const QString& _user, TcpClient* _tsocket, QWidget *parent = nullptr);
    ~Menu();


private slots:
    void showMessagesPage();
    void showFunctionsPage();
    void onFriendChatButtonClicked();
    void onGroupChatButtonClicked();
    void onAddFriendButtonClicked();
    void fetchMessages();
    void showMessageDetails(QListWidgetItem *item);



private:
    void setupUI();
    void setupConnections();


    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

    QPushButton *messageButton;
    QPushButton *functionButton;
    QWidget *messagesPage;
    QWidget *functionsPage;
    QStackedWidget *stackedWidget;
    QVBoxLayout *messagesLayout;
    QVBoxLayout *functionsLayout;
    QLabel *userAvatar;
    QListWidget *messageListWidget;
    QVBoxLayout *mainLayout;

    QString user;
    TcpClient *tsocket;

};

#endif // MENU_H
