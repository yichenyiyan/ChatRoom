#ifndef GROUPREQUESTWINDOW_H
#define GROUPREQUESTWINDOW_H




#include <QWidget>
#include <QPushButton>
#include <QLabel>

class GroupRequestWindow : public QWidget {
    Q_OBJECT

public:
    explicit GroupRequestWindow(const QString& _master_user, const QString& _req_user, const QString& _group_name, QWidget *parent = nullptr);
    ~GroupRequestWindow();

private slots:
    void on_acceptButton_clicked();
    void on_rejectButton_clicked();

private:
    void centerWindow();

private:
    QString req_user;
    QString master_user;
    QString group_name;

    QLabel *messageLabel;
    QPushButton *acceptButton;
    QPushButton *rejectButton;
};


#endif // GROUPREQUESTWINDOW_H
