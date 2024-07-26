#ifndef LOGIN_H
#define LOGIN_H

#include <QString>
#include <QString>
#include <QWidget>
#include <QScreen>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>

// 登录
class Login : public QWidget {
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    void centerWindow();
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_loginButton_clicked();
    void on_backButton_clicked(); // Slot for back button

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *backButton; // Back button

//signals:
//    void showMenu();
//    void toBack();
};

#endif // LOGIN_H
