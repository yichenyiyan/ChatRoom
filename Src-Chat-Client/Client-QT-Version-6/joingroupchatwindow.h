#ifndef JOINGROUPCHATWINDOW_H
#define JOINGROUPCHATWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class TcpClient;

class JoinGroupChatWindow : public QWidget {
    Q_OBJECT

public:
    explicit JoinGroupChatWindow(const QString& user, QWidget *parent = nullptr);
    ~JoinGroupChatWindow();

private slots:
    void onJoinGroupButtonClicked();
    void on_GroupLineEdit_textChanged(const QString &text);

private:
    void centerWindow();

private:
    QString JoinUser;
    TcpClient* sendSocket;
    QLineEdit *groupNameLineEdit;
    QPushButton *joinGroupButton;
};

#endif // JOINGROUPCHATWINDOW_H
