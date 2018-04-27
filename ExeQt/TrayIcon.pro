#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T20:32:40
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrayIcon
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwidget.cpp \
    actiontab.cpp \
    commandaction.cpp \
    action.cpp \
    addtab.cpp \
    actionitem.cpp \
    addgroupdialog.cpp \
    applicationaction.cpp \
    menuaction.cpp \
    saveable.cpp \
    groupconfigure.cpp \
    linkaction.cpp \
    bundle.cpp

HEADERS += \
        mainwidget.h \
    actiontab.h \
    commandaction.h \
    action.h \
    addtab.h \
    actionitem.h \
    addgroupdialog.h \
    applicationaction.h \
    menuaction.h \
    saveable.h \
    groupconfigure.h \
    linkaction.h \
    bundle.h \
    appicon.h

FORMS += \
        mainwidget.ui \
    actiontab.ui \
    commandaction.ui \
    addtab.ui \
    actionitem.ui \
    addgroupdialog.ui \
    applicationaction.ui \
    groupconfigure.ui \
    linkaction.ui

RESOURCES += \
    resources.qrc

#unix:!macx: LIBS += -L$$PWD/libs/singleapplication/lib/ -lQt5Solutions_SingleApplication-head

#include(/data/Libraries/qt-solutions-master/qtsingleapplication/src/qtsingleapplication.pri)

#INCLUDEPATH += $$PWD/libs/singleapplication/include
#DEPENDPATH += $$PWD/libs/singleapplication/include

include($$PWD/libs/singleapplication/src/qtsingleapplication.pri)
