#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>

class AddFriend : public QWidget {
    Q_OBJECT

public:
    explicit AddFriend(QWidget *parent = nullptr);
    ~AddFriend();

private slots:
    void on_addFriendButton_clicked();

private:
    QLineEdit *requestUserLineEdit;
    QLineEdit *friendUserLineEdit;
    QPushButton *addFriendButton;
    QTcpSocket *socket;
};

#endif // ADDFRIEND_H
