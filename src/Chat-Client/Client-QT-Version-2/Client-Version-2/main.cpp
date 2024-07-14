#include "yichen_dialog.h"


#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("raw_man.jpg"); //启动程序前显示的图片
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();

    yichen_dialog w;
    w.show();
    splash.finish(&w);

    return a.exec();
}
