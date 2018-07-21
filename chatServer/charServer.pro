TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT  +=  network
QT += sql
QT += gui

SOURCES += main.cpp \
    clientuser.cpp \
    tcpclientsocket.cpp \
    tcpgroup.cpp \
    tcpserver.cpp \
    tcpuser.cpp \
    tcpmoodmsg.cpp

HEADERS += \
    clientuser.h \
    cmd.h \
    tcpclientsocket.h \
    tcpgroup.h \
    tcpserver.h \
    tcpuser.h \
    tcpmoodmsg.h
