#ifndef GROUPLISTPAGE_H
#define GROUPLISTPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QApplication>
#include <QScreen>

class Menu;

class GroupListPage : public QWidget {
    Q_OBJECT

public:
    explicit GroupListPage(Menu* _menu, QWidget *parent = nullptr);
    ~GroupListPage();
private:
    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }
private slots:
    void onGroupClicked(QListWidgetItem *item);

private:
    void loadGroupList();

    QVBoxLayout *layout;
    QListWidget *groupListWidget;
    QPushButton *backButton;

    Menu* menu;
};

#endif // GROUPLISTPAGE_H
