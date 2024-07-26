#ifndef CREATEGROUPCHATWINDOW_H
#define CREATEGROUPCHATWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class TcpClient;

class CreateGroupChatWindow : public QWidget {
    Q_OBJECT

public:
    explicit CreateGroupChatWindow(const QString& user, QWidget *parent = nullptr);
    ~CreateGroupChatWindow();

private slots:
    void onCreateGroupButtonClicked();

private:
    void centerWindow();

private:
    QString CreateUser;
    TcpClient* sendSocket;
    QLineEdit *groupNameLineEdit;
    QLineEdit *groupDescLineEdit;
    QPushButton *createGroupButton;
};

#endif // CREATEGROUPCHATWINDOW_H
