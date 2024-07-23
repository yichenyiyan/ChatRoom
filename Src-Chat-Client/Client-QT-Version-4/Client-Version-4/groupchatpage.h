#ifndef GROUPCHATPAGE_H
#define GROUPCHATPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QApplication>
#include <QScreen>
class Menu;

class GroupChatPage : public QWidget {
    Q_OBJECT

public:
    explicit GroupChatPage(const QString &groupName, Menu* _menu, QWidget *parent = nullptr);
    ~GroupChatPage();
private:
    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }
private slots:
    void onSendMessage();

private:
    QString groupName;
    QVBoxLayout *layout;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *backButton;
    Menu *menu;
};

#endif // GROUPCHATPAGE_H
