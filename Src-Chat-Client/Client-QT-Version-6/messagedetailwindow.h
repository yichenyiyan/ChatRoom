#ifndef MESSAGEDETAILWINDOW_H
#define MESSAGEDETAILWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QScrollArea>
#include <QMap>
#include <QList>

class TcpClient;

class MessageDetailWindow : public QWidget {
    Q_OBJECT

public:
    explicit MessageDetailWindow(const QString& _sendUsername,const QString &username, bool isGroup = false, QWidget *parent = nullptr);

public slots:
    void addMessage(const QString &username, const QString &message, bool isOwnMessage = false);

private:
    void setupUI();

private:
    bool isGroupWin;
    QString sendUsername;
    QString friendUserName;
    TcpClient* sendSocket;
    QVBoxLayout *mainLayout;
    QVBoxLayout *messagesLayout;
    QScrollArea *scrollArea;
    QWidget *messagesContainer;
    QTextEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *backButton;

};

#endif // MESSAGEDETAILWINDOW_H
