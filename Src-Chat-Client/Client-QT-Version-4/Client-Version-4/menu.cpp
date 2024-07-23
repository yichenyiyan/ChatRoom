#include "menu.h"
#include "tcpclient.h"
#include "friendlistpage.h"
#include "grouplistpage.h"
#include "privatechatpage.h"
#include "addfriendpage.h"
#include "groupchatpage.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QListWidgetItem>
#include <QMessageBox>

Menu::Menu(const QString& _user, TcpClient* _tsocket,  QWidget *parent)
    : QWidget(parent), user(_user), tsocket(_tsocket)  {
    this->setFixedSize(400, 600);
    centerWindow();

    setupUI();
    setupConnections();

    // 默认显示消息页面
    stackedWidget->setCurrentWidget(messagesPage);

    // 模拟从服务器获取消息
    fetchMessages();
}

Menu::~Menu() {}

void Menu::setupUI() {
    // 创建切换按钮
    messageButton = new QPushButton("消息", this);
    functionButton = new QPushButton("功能", this);

    // 设置按钮样式透明化
    messageButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");
    functionButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");

    // 创建用户头像
    userAvatar = new QLabel(this);
    userAvatar->setPixmap(QPixmap(":/images/avatar.png").scaled(40, 40, Qt::KeepAspectRatio));
    userAvatar->setFixedSize(40, 40);

    // 创建页面
    messagesPage = new QWidget(this);
    functionsPage = new QWidget(this);

    // 设置消息页面布局
    messagesLayout = new QVBoxLayout(messagesPage);
    messageListWidget = new QListWidget(this);
    messagesLayout->addWidget(messageListWidget);

    // 设置功能页面布局
    functionsLayout = new QVBoxLayout(functionsPage);
    QPushButton *friendChatButton = new QPushButton("好友聊天", functionsPage);
    QPushButton *addFriendButton = new QPushButton("添加好友", functionsPage);
    QPushButton *groupChatButton = new QPushButton("群聊聊天", functionsPage);
    functionsLayout->addWidget(friendChatButton);
    functionsLayout->addWidget(addFriendButton);
    functionsLayout->addWidget(groupChatButton);

    // 创建QStackedWidget并添加页面
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(messagesPage);
    stackedWidget->addWidget(functionsPage);

    // 创建主布局并添加组件
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(messageButton);
    buttonLayout->addWidget(functionButton);
    mainLayout->addLayout(buttonLayout);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(new QLabel("", this));
    topLayout->addStretch();
    topLayout->addWidget(userAvatar);

    mainLayout->insertLayout(0, topLayout);

    setLayout(mainLayout);


}

void Menu::setupConnections() {
    connect(messageButton, &QPushButton::clicked, this, &Menu::showMessagesPage);
    connect(functionButton, &QPushButton::clicked, this, &Menu::showFunctionsPage);

    connect(static_cast<QPushButton*>(functionsLayout->itemAt(0)->widget()), &QPushButton::clicked, this, &Menu::onFriendChatButtonClicked);
    connect(static_cast<QPushButton*>(functionsLayout->itemAt(1)->widget()), &QPushButton::clicked, this, &Menu::onAddFriendButtonClicked);
    connect(static_cast<QPushButton*>(functionsLayout->itemAt(2)->widget()), &QPushButton::clicked, this, &Menu::onGroupChatButtonClicked);

    connect(messageListWidget, &QListWidget::itemClicked, this, &Menu::showMessageDetails);
}

void Menu::showMessagesPage() {
    stackedWidget->setCurrentWidget(messagesPage);
}

void Menu::showFunctionsPage() {
    stackedWidget->setCurrentWidget(functionsPage);
}

void Menu::onFriendChatButtonClicked() {
    // 好友聊天
    this->hide();
    FriendListPage *friendListPage = new FriendListPage(this, tsocket, user);
    friendListPage->show();
}

void Menu::onAddFriendButtonClicked() {
    // 添加好友
    this->hide();
    AddFriendPage *addFriendPage = new AddFriendPage(this);
    addFriendPage->show();
}

void Menu::onGroupChatButtonClicked() {
    // 群聊聊天
    this->hide();
    GroupListPage *groupListPage = new GroupListPage(this);
    groupListPage->show();
}

void Menu::fetchMessages() {
    QString cmd = "";
    tsocket->sendCommand(cmd);
    QObject::connect(tsocket, &TcpClient::receivedMessage, [&](){
        QString messages = tsocket->getMessage();

        QStringList words = messages.split(" ");

        QStringList wordsWithRegex = messages.split(QRegExp("\\s+"));
        // 遍历并打印结果
        foreach(const QString &word, wordsWithRegex) {
            qDebug() << word;
        }
    });



}

void Menu::showMessageDetails(QListWidgetItem *item) {
    // 显示消息详情
    QString message = item->text();
    QMessageBox::information(this, "消息详情", message);
}




