#ifndef ADDFRIENDWINDOW_H
#define ADDFRIENDWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>

class TcpClient;

class AddFriendWindow : public QWidget {
    Q_OBJECT

public:
    explicit AddFriendWindow(const QString& _user, QWidget *parent = nullptr);
    ~AddFriendWindow();

private slots:
    void on_addFriendButton_clicked();
    void on_friendUserLineEdit_textChanged(const QString &text);
    void on_receiveResponse();

private:
    void centerWindow();

private:
    QString user;
    TcpClient* sendSocket;
    QLineEdit *friendUserLineEdit;
    QPushButton *addFriendButton;
};

#endif // ADDFRIENDWINDOW_H
