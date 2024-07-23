#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

class TcpClient;

class Chat : public QWidget {
    Q_OBJECT

public:
    Chat(QWidget *parent = nullptr, TcpClient* _tsocket = nullptr,
         QString src_user = "", QString target_user = "");
    ~Chat();

private slots:
    void on_sendMessageButton_clicked();

private:
    QLineEdit *messageLineEdit;
    QPushButton *sendMessageButton;
    QTextEdit *chatDisplay;

    TcpClient *tsocket;
    QString *sender;
    QString *receiver;
};

#endif // CHAT_H
