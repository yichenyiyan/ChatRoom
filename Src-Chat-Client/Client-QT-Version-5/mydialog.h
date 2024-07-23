#ifndef MYQDIALOG_H
#define MYQDIALOG_H

#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>


#include <QDialog>

class MyQDialog : public QDialog
{
    Q_OBJECT

public:
    MyQDialog();

public slots:

    void setpass(bool flag){

      if(!flag)
             LineEdit1->setEchoMode(QLineEdit::Password);
       else
             LineEdit1->setEchoMode(QLineEdit::Normal);
    }

public slots:

    void loading_push(){
        stat = true;
        close();
    }

public:

    static int getstat(){

        MyQDialog  da;
        da.setFixedSize(600,400);
        da.exec();

        return da.stat;

    }

private:
        int stat;
        QLineEdit *LineEdit0,*LineEdit1;
        QCheckBox *CheckBox;
        QPushButton *PushButton;
        QRadioButton *RadioButton;
        QLabel *Label0,*Label1;


};

#endif // MYQDIALOG_H

