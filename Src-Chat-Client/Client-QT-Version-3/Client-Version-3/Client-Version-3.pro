QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = yichenChatRoom
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    yichen_addfriends.cpp \
    yichen_drawer.cpp \
    yichen_sign.cpp \
    yichen_tcp.cpp \

HEADERS += \
    yichen_addfriends.h \
    yichen_command.h \
    yichen_drawer.h \
    yichen_sign.h \
    yichen_tcp.h \


RESOURCES += \
    yichen_images.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
