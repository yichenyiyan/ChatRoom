QT       += core gui
QT       += core gui network
QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriendwindow.cpp \
    creategroupchatwindow.cpp \
    friendlistwindow.cpp \
    friendrequestwindow.cpp \
    grouplistwindow.cpp \
    grouprequestwindow.cpp \
    joingroupchatwindow.cpp \
    loginwindow.cpp \
    main.cpp \
    menu.cpp \
    messagedetailwindow.cpp \
    messagelistwindow.cpp \
    registerwindow.cpp \
    tcpclient.cpp \
    tcpclientthread.cpp

HEADERS += \
    addfriendwindow.h \
    creategroupchatwindow.h \
    friendlistwindow.h \
    friendrequestwindow.h \
    grouplistwindow.h \
    grouprequestwindow.h \
    joingroupchatwindow.h \
    loginwindow.h \
    menu.h \
    messagedetailwindow.h \
    messagelistwindow.h \
    registerwindow.h \
    tcpclient.h \
    tcpclientthread.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
