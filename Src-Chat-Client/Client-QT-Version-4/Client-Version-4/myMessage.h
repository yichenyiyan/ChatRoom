#ifndef MYMESSAGE_H
#define MYMESSAGE_H

#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

class CustomMessageBox : public QMessageBox {
    Q_OBJECT
public:
    CustomMessageBox(QWidget *parent = nullptr) : QMessageBox(parent) {
        this->setFixedSize(200, 200);
    }

protected:
    void showEvent(QShowEvent *event) override {
        QMessageBox::showEvent(event);
        this->setFixedSize(200, 200);
    }
};


#endif // MYMESSAGE_H
