#-------------------------------------------------
#
# Project created by QtCreator 2015-02-16T12:31:03
#
#-------------------------------------------------

QT       += core gui

TARGET = zigi
TEMPLATE = app
LIBS += -lbluetooth

SOURCES += main.cpp\
        mainwindow.cpp \
    core.cpp \
    dialogprogress.cpp \
    dialogselectlist.cpp \
    systhread.cpp \
    bluetooth.cpp \
    coreadapter.cpp

HEADERS  += mainwindow.h \
    core.h \
    dialogprogress.h \
    dialogselectlist.h \
    systhread.h \
    bluetooth.h \
    coreadapter.h \
    const.h

FORMS    += mainwindow.ui \
    dialogprogress.ui \
    dialogselectlist.ui

unix: CONFIG += link_pkgconfig

RESOURCES += \
    res.qrc
