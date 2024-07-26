#ifndef ENTRY_H
#define ENTRY_H

#include <QWidget>
#include <QScreen>
#include <QApplication>
#include <QCloseEvent>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class Entry : public QMainWindow
{
    Q_OBJECT
public:
    Entry(QWidget *parent = nullptr);
    ~Entry();

private:
    void closeEvent(QCloseEvent *event) override;
    void centerWindow();

private slots:
    void to_showMenu();
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    QPushButton* loginButton;
    QPushButton* registerButton;
};


#endif // ENTRY_H
