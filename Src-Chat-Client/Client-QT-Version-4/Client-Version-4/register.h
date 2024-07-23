#ifndef REGISTER_H
#define REGISTER_H

#include <QApplication>
#include <QScreen>
#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>

class TcpClient;

// 注册
class Register : public QWidget {
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private:
    inline void centerWindow() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - this->width()) / 2;
        int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    }

private slots:
    void on_registerButton_clicked();
    void on_backButton_clicked(); // Slot for back button

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *registerButton;
    QPushButton *backButton; // Back button
    TcpClient* tsocket;

signals:
    void back();
    void toBack();
};

#endif // REGISTER_H
