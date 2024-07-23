#include "mydialog.h"


#include <QVBoxLayout>
#include <QHBoxLayout>

MyQDialog::MyQDialog()
{
       Label0 = new QLabel("welcome useing my QQ");
       Label1 = new QLabel;
       QPixmap pix("1.png");
       Label1->setFixedSize(200,250);
       Label1->setScaledContents(true);
       Label1->setPixmap(pix);

       QVBoxLayout *vbox0 = new QVBoxLayout;
       vbox0->addWidget(Label1);
       vbox0->addWidget(Label0);


       LineEdit0 = new QLineEdit;
       LineEdit0->setFixedSize(300,50);
       LineEdit1 = new QLineEdit;
       LineEdit1->setFixedSize(300,50);
       LineEdit1->setEchoMode(QLineEdit::Password);
       CheckBox = new QCheckBox("show password");
       PushButton = new QPushButton("Lading");
       PushButton->setFixedSize(200,50);
       RadioButton = new QRadioButton("I accept this!");
       QVBoxLayout *vbox1 = new QVBoxLayout;
       vbox1->addWidget(LineEdit0);
       vbox1->addWidget(LineEdit1);
       vbox1->addWidget(CheckBox);
       vbox1->addWidget(RadioButton);
       vbox1->addWidget(PushButton);

       QHBoxLayout *hbox = new QHBoxLayout;

       hbox->addLayout(vbox0);
       hbox->addLayout(vbox1);


       this->setLayout(hbox);

       connect(CheckBox,SIGNAL(clicked(bool)),this ,SLOT(setpass(bool)));

       stat = false;

       connect(PushButton,SIGNAL(clicked(bool)),this ,SLOT(loading_push()));



}

