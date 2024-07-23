#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class TcpClient;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
private slots:
    void ShowMenu(QString user);
    void toVerifyLogin();
    void togglePasswordVisibility();
    void openRegisterWindow();

private:
    TcpClient *tcpclient;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *togglePasswordButton;

private:
    void centerWindow();

signals:
    void toShowMenu(QString user);
};

#endif // LOGINWINDOW_H
