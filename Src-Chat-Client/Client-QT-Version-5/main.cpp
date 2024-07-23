#include <QApplication>
#include "menu.h"
#include "friendrequestwindow.h"
#include "addfriendwindow.h"
#include "loginwindow.h"
#include "messagelistwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LoginWindow loginWindow;
    loginWindow.show();

//    MessageListWindow window;
//    window.show();

//    AddFriendWindow w;
//    w.show();

//    FriendRequestWindow f("yichen", "jiedan");
//    f.show();
    return app.exec();
}
