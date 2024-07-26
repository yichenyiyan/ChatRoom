#include "widget.h"
#include <QDialog>

#include "mydialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
  int stat = MyQDialog::getstat();
  if(!stat)
            exit(0);

  Label0 = new QLabel;
  QPixmap pix("2.png");
  Label0->setFixedSize(150,150);
  Label0->setScaledContents(true);
  Label0->setPixmap(pix);
  QHBoxLayout *Hbox = new QHBoxLayout;
  Hbox->addWidget(Label0);


  Label1 = new QLabel("lineing");
  Hbox->addWidget(Label0);


  ToolBox0 = new QToolBox;
  ToolBox1 = new QToolBox;
  ToolBox2 = new QToolBox;
  pb0 = new QPushButton("wzfdashuaibi");
  pb1 = new QPushButton("好友2");
  pb2 = new QPushButton("好友3");
  pb3 = new QPushButton("好友4");
  pb4 = new QPushButton("好友5");
  pb5 = new QPushButton("好友6");
  pb6 = new QPushButton("好友7");
  pb7 = new QPushButton("好友8");

  ToolBox0->addItem(pb0,"groap1");


  ToolBox1->addItem(pb1,"groap1");
  ToolBox1->addItem(pb2,"groap1");
  ToolBox1->addItem(pb3,"groap2");
  ToolBox1->addItem(pb4,"groap2");

  ToolBox2->addItem(pb5,"groap2");
  ToolBox2->addItem(pb6,"groap3");
  ToolBox2->addItem(pb7,"groap3");

  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(ToolBox0);
  vbox->addWidget(ToolBox1);
  vbox->addWidget(ToolBox2);

  QVBoxLayout *mainvbox = new QVBoxLayout;
  mainvbox->addLayout(Hbox);
  mainvbox->addLayout(vbox);

  this->setLayout(mainvbox);







}

Widget::~Widget()
{

}
