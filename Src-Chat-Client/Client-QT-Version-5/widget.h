#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QTabWidget>
#include <QToolBox>
#include <QLabel>
#include <QPushButton>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();



private:
    QLabel *Label0,*Label1;
    QToolBox *ToolBox0,*ToolBox1,*ToolBox2;
   // QTabWidget *TabWidget0,*TabWidget1,*TabWidget2;
    QPushButton *pb0,*pb1,*pb2,*pb3,*pb4,*pb5,*pb6,*pb7;


};

#endif // WIDGET_H
