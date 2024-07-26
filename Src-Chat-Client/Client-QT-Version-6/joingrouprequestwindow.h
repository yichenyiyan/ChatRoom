#ifndef JOINGROUPREQUESTWINDOW_H
#define JOINGROUPREQUESTWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class TcpClient;

class JoinGroupRequestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit JoinGroupRequestWindow(const QString& _user, QWidget *parent = nullptr);

private slots:
    void onSubmit();

private:
    QString user;
    TcpClient* sendSocket;
    QLineEdit *usernameEdit;
    QLineEdit *groupnameEdit;
    QPushButton *submitButton;
};

#endif // JOINGROUPREQUESTWINDOW_H
