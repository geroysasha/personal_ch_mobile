#-------------------------------------------------
#
# Project created by QtCreator 2015-02-16T12:31:03
#
#-------------------------------------------------

QT       += core gui

TARGET = zigi
TEMPLATE = app

LIBS += -lbluetooth \
    -lpjsua-i686-pc-linux-gnu \
    -lpjsip-ua-i686-pc-linux-gnu \
    -lpjsip-simple-i686-pc-linux-gnu \
    -lpjsip-i686-pc-linux-gnu \
    -lpjmedia-codec-i686-pc-linux-gnu \
    -lpjmedia-i686-pc-linux-gnu \
    -lpjmedia-audiodev-i686-pc-linux-gnu \
    -lpjnath-i686-pc-linux-gnu \
    -lpjlib-util-i686-pc-linux-gnu \
    -lpj-i686-pc-linux-gnu \
    -lportaudio-i686-pc-linux-gnu \
    -lgsmcodec-i686-pc-linux-gnu \
    -lilbccodec-i686-pc-linux-gnu \
    -lspeex-i686-pc-linux-gnu \
    -lresample-i686-pc-linux-gnu \
    -lsrtp-i686-pc-linux-gnu \
    -lm \
    -lpthread \
    -lssl \
    -lasound \

SOURCES += main.cpp\
        mainwindow.cpp \
    core.cpp \
    dialogprogress.cpp \
    dialogselectlist.cpp \
    systhread.cpp \
    bluetooth.cpp \
    mobilechannel.cpp \
    dialogalert.cpp \
    posthttp.cpp \
    calltimer.cpp

HEADERS  += mainwindow.h \
    core.h \
    dialogprogress.h \
    dialogselectlist.h \
    systhread.h \
    bluetooth.h \
    const.h \
    mobilechannel.h \
    dialogalert.h \
    posthttp.h \
    calltimer.h

FORMS    += mainwindow.ui \
    dialogprogress.ui \
    dialogselectlist.ui \
    dialogalert.ui

unix: CONFIG += link_pkgconfig

RESOURCES += \
    res.qrc
