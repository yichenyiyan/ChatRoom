#ifndef LOGIN_H
#define LOGIN_H

#include <QApplication>
#include <QWidget>
#include <QScreen>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>

class TcpClient;

// 登录
class Login : public QWidget {
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
private:
    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

private slots:
    void toShowMenu(const QString& user);
    void on_loginButton_clicked();
    void on_backButton_clicked(); // Slot for back button

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *backButton; // Back button
    TcpClient *tsocket;

signals:
    void toBack();
};

#endif // LOGIN_H
