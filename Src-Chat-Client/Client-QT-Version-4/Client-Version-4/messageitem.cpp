#include "messageitem.h"

MessageItem::MessageItem(const QString &message, QWidget *parent)
    : QPushButton(message, parent), message(message) {
    setStyleSheet("QPushButton { background: transparent; color: black; border: none; }");
    connect(this, &QPushButton::clicked, this, &MessageItem::clicked);
}
