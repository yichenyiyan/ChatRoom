#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MessageItem : public QPushButton {
    Q_OBJECT
public:
    explicit MessageItem(const QString &message, QWidget *parent = nullptr);

signals:
    void clicked();

private:
    QString message;
};

#endif // MESSAGEITEM_H
