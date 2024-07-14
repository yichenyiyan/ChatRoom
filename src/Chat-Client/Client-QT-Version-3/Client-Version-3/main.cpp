#include "yichen_drawer.h"
#include "yichen_sign.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    yichen_sign s;
    s.show();

    return a.exec();
}
