#include "menu.h"
#include "tcpclient.h"
#include "messagelistwindow.h"
#include "friendlistwindow.h"
#include "addfriendwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QListWidgetItem>
#include <QMessageBox>

Menu::Menu(const QString& _user, QWidget *parent)
    : QWidget(parent), user(_user)  {
    this->setWindowTitle(user);
    this->setFixedSize(430, 620);
    centerWindow();
    setStyleSheet("background-color: lightblue;");

    tcpclient = new TcpClient();

    setupUI();
    setupConnections();

    // 默认显示消息页面
    stackedWidget->setCurrentWidget(messagesPage);
}


Menu::~Menu() {}

void Menu::setupUI() {
    // 创建切换按钮
    messageButton = new QPushButton("消息", this);
    functionButton = new QPushButton("功能", this);
    QString buttonStyle = "QPushButton { border: 2px solid #8f8f91; border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); }";
    messageButton->setStyleSheet(buttonStyle);
    functionButton->setStyleSheet(buttonStyle);


    // 创建页面
    messagesPage = new MessageListWindow(user,this);
    functionsPage = new QWidget(this);
    messagesPage->setStyleSheet(buttonStyle);
    functionsPage->setStyleSheet(buttonStyle);


    // 设置功能页面布局
    functionsLayout = new QVBoxLayout(functionsPage);
    QPushButton *friendChatButton = new QPushButton("好友聊天", functionsPage);
    QPushButton *addFriendButton = new QPushButton("添加好友", functionsPage);
    QPushButton *groupChatButton = new QPushButton("群聊聊天", functionsPage);

    friendChatButton->setStyleSheet(buttonStyle);
    addFriendButton->setStyleSheet(buttonStyle);
    groupChatButton->setStyleSheet(buttonStyle);

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

    setLayout(mainLayout);
}

void Menu::setupConnections() {
    connect(messageButton, &QPushButton::clicked, this, &Menu::showMessagesPage);
    connect(functionButton, &QPushButton::clicked, this, &Menu::showFunctionsPage);

    connect(static_cast<QPushButton*>(functionsLayout->itemAt(0)->widget()), &QPushButton::clicked, this, &Menu::onFriendChatButtonClicked);
    connect(static_cast<QPushButton*>(functionsLayout->itemAt(1)->widget()), &QPushButton::clicked, this, &Menu::onAddFriendButtonClicked);
    connect(static_cast<QPushButton*>(functionsLayout->itemAt(2)->widget()), &QPushButton::clicked, this, &Menu::onGroupChatButtonClicked);

}

void Menu::showMessagesPage() {
    stackedWidget->setCurrentWidget(messagesPage);
}

void Menu::showFunctionsPage() {
    stackedWidget->setCurrentWidget(functionsPage);
}

void Menu::onFriendChatButtonClicked() {
    // 好友聊天
    FriendListWindow* _friends = new FriendListWindow(user, messagesPage, nullptr);
    _friends->show();
}

void Menu::onAddFriendButtonClicked() {
    // 添加好友
    AddFriendWindow* addFriendWin = new AddFriendWindow(user, nullptr);
    addFriendWin->show();
}

void Menu::onGroupChatButtonClicked() {
    // 群聊聊天

}

void Menu::fetchMessages() {
    //
}


void Menu::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}


