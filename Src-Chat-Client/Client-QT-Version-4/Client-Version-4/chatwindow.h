#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ChatWindow : public QWidget {
    Q_OBJECT
public:
    explicit ChatWindow(QWidget *parent = nullptr);

public slots:
    void setChatDetails(const QString &details);

private slots:
    void onSendButtonClicked();
    void onReturnButtonClicked();

private:
    QTextEdit *chatDisplay;
    QTextEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *returnButton;
};

#endif // CHATWINDOW_H
