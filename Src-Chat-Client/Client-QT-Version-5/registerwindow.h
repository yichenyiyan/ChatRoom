#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>

class TcpClient;

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void togglePasswordVisibility();
    void registerUser();

private:
    TcpClient *tcpclient;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *togglePasswordButton;

private:
    void centerWindow();
    void closeEvent(QCloseEvent *event) override;
};

#endif // REGISTERWINDOW_H
