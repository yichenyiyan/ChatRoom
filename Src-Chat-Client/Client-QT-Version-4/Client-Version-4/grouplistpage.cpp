#include "menu.h"
#include "grouplistpage.h"
#include "groupchatpage.h"

GroupListPage::GroupListPage(Menu* _menu, QWidget *parent)
    : QWidget(parent), menu(_menu) {
    this->setFixedSize(400, 600);
    centerWindow();

    layout = new QVBoxLayout(this);

    groupListWidget = new QListWidget(this);
    layout->addWidget(groupListWidget);

    backButton = new QPushButton("返回", this);
    layout->addWidget(backButton);

    connect(groupListWidget, &QListWidget::itemClicked, this, &GroupListPage::onGroupClicked);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        this->close();
        menu->show();
    });

    loadGroupList();
}

GroupListPage::~GroupListPage() {}

void GroupListPage::onGroupClicked(QListWidgetItem *item) {
    QString groupName = item->text();
    this->hide();
    GroupChatPage *groupChatPage = new GroupChatPage(groupName, menu);
    groupChatPage->show();
}

void GroupListPage::loadGroupList() {
    // 模拟从服务器获取数据
    QStringList groups = {"Group1", "Group2", "Group3"};
    for (const QString &groupName : groups) {
        QListWidgetItem *item = new QListWidgetItem(groupName, groupListWidget);
        item->setText(groupName);
    }
}
