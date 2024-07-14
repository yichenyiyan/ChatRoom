/********************************************************************************
** Form generated from reading UI file 'yichen_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YICHEN_WIDGET_H
#define UI_YICHEN_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextBrowser *msgBrowser;
    QTextEdit *msgTxtEdit;
    QTableWidget *usrTblWidget;
    QFontComboBox *fontCbx;
    QComboBox *sizeCbx;
    QPushButton *sendBtn;
    QPushButton *exitBtn;
    QLabel *usrNumLbl;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(780, 450);
        msgBrowser = new QTextBrowser(Widget);
        msgBrowser->setObjectName(QString::fromUtf8("msgBrowser"));
        msgBrowser->setGeometry(QRect(5, 11, 541, 231));
        msgTxtEdit = new QTextEdit(Widget);
        msgTxtEdit->setObjectName(QString::fromUtf8("msgTxtEdit"));
        msgTxtEdit->setGeometry(QRect(3, 280, 541, 131));
        usrTblWidget = new QTableWidget(Widget);
        if (usrTblWidget->columnCount() < 2)
            usrTblWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        usrTblWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        usrTblWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        usrTblWidget->setObjectName(QString::fromUtf8("usrTblWidget"));
        usrTblWidget->setGeometry(QRect(555, 11, 221, 401));
        usrTblWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        usrTblWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        fontCbx = new QFontComboBox(Widget);
        fontCbx->setObjectName(QString::fromUtf8("fontCbx"));
        fontCbx->setGeometry(QRect(0, 250, 213, 22));
        sizeCbx = new QComboBox(Widget);
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->setObjectName(QString::fromUtf8("sizeCbx"));
        sizeCbx->setGeometry(QRect(230, 250, 69, 22));
        sendBtn = new QPushButton(Widget);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));
        sendBtn->setGeometry(QRect(460, 420, 75, 23));
        exitBtn = new QPushButton(Widget);
        exitBtn->setObjectName(QString::fromUtf8("exitBtn"));
        exitBtn->setGeometry(QRect(560, 420, 75, 23));
        usrNumLbl = new QLabel(Widget);
        usrNumLbl->setObjectName(QString::fromUtf8("usrNumLbl"));
        usrNumLbl->setGeometry(QRect(660, 420, 91, 20));

        retranslateUi(Widget);

        sizeCbx->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = usrTblWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Widget", "\346\226\260\345\273\272\345\210\227", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = usrTblWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Widget", "ipAddress", nullptr));
        sizeCbx->setItemText(0, QCoreApplication::translate("Widget", "8", nullptr));
        sizeCbx->setItemText(1, QCoreApplication::translate("Widget", "9", nullptr));
        sizeCbx->setItemText(2, QCoreApplication::translate("Widget", "10", nullptr));
        sizeCbx->setItemText(3, QCoreApplication::translate("Widget", "11", nullptr));
        sizeCbx->setItemText(4, QCoreApplication::translate("Widget", "12", nullptr));
        sizeCbx->setItemText(5, QCoreApplication::translate("Widget", "13", nullptr));
        sizeCbx->setItemText(6, QCoreApplication::translate("Widget", "14", nullptr));
        sizeCbx->setItemText(7, QCoreApplication::translate("Widget", "15", nullptr));
        sizeCbx->setItemText(8, QCoreApplication::translate("Widget", "16", nullptr));
        sizeCbx->setItemText(9, QCoreApplication::translate("Widget", "17", nullptr));
        sizeCbx->setItemText(10, QCoreApplication::translate("Widget", "18", nullptr));
        sizeCbx->setItemText(11, QCoreApplication::translate("Widget", "19", nullptr));
        sizeCbx->setItemText(12, QCoreApplication::translate("Widget", "20", nullptr));
        sizeCbx->setItemText(13, QCoreApplication::translate("Widget", "21", nullptr));
        sizeCbx->setItemText(14, QCoreApplication::translate("Widget", "22", nullptr));

        sendBtn->setText(QCoreApplication::translate("Widget", "Send", nullptr));
        exitBtn->setText(QCoreApplication::translate("Widget", "Exit", nullptr));
        usrNumLbl->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YICHEN_WIDGET_H
