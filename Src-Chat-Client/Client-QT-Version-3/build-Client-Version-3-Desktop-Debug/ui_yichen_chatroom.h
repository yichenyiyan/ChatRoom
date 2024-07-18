/********************************************************************************
** Form generated from reading UI file 'yichen_chatroom.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YICHEN_CHATROOM_H
#define UI_YICHEN_CHATROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_yichen_chatroom
{
public:

    void setupUi(QDialog *yichen_chatroom)
    {
        if (yichen_chatroom->objectName().isEmpty())
            yichen_chatroom->setObjectName(QString::fromUtf8("yichen_chatroom"));
        yichen_chatroom->resize(800, 600);

        retranslateUi(yichen_chatroom);

        QMetaObject::connectSlotsByName(yichen_chatroom);
    } // setupUi

    void retranslateUi(QDialog *yichen_chatroom)
    {
        yichen_chatroom->setWindowTitle(QCoreApplication::translate("yichen_chatroom", "yichen_chatroom", nullptr));
    } // retranslateUi

};

namespace Ui {
    class yichen_chatroom: public Ui_yichen_chatroom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YICHEN_CHATROOM_H
