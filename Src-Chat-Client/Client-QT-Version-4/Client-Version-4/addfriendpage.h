#ifndef ADDFRIENDPAGE_H
#define ADDFRIENDPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QApplication>
#include <QScreen>

class Menu;

class AddFriendPage : public QWidget {
    Q_OBJECT

public:
    explicit AddFriendPage(Menu* _menu, QWidget *parent = nullptr);
    ~AddFriendPage();
private:
    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }
private slots:
    void onAddFriend();

private:
    QVBoxLayout *layout;
    QLineEdit *friendNameInput;
    QPushButton *addButton;
    QPushButton *backButton;
    QLabel *statusLabel;

    Menu* menu;
};

#endif // ADDFRIENDPAGE_H
