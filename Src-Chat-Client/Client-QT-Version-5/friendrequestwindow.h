#ifndef FRIENDREQUESTWINDOW_H
#define FRIENDREQUESTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class FriendRequestWindow : public QWidget {
    Q_OBJECT

public:
    explicit FriendRequestWindow(const QString& _this_username, const QString& username, QWidget *parent = nullptr);
    ~FriendRequestWindow();

private slots:
    void on_acceptButton_clicked();
    void on_rejectButton_clicked();

private:
    void centerWindow();

private:
    QString username;
    QString this_username;
    QLabel *messageLabel;
    QPushButton *acceptButton;
    QPushButton *rejectButton;
};

#endif // FRIENDREQUESTWINDOW_H
