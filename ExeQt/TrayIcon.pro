#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T20:32:40
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrayIcon
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

@
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
@

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
    bundle.cpp \
    logindialog.cpp \
    requestmanager.cpp \
    mainpage.cpp \
    authmanager.cpp \
    constants.cpp \
    stylemanager.cpp \
    singleinstanceapplication.cpp \
    settingsdialog.cpp \
    setaction.cpp \
    actionsettree.cpp \
    task.cpp \
    remotecontrol.cpp \
    remoteauthorizations.cpp \
    remotecontroltab.cpp \
    remoteauthtab.cpp \
    networkmanager.cpp \
    authconnecttab.cpp \
    networkmessage.cpp \
    actionreference.cpp \
    clientinfodialog.cpp \
    settingsregistry.cpp

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
    appicon.h \
    logindialog.h \
    requestmanager.h \
    mainpage.h \
    constants.h \
    authmanager.h \
    stylemanager.h \
    singleinstanceapplication.h \
    settingsdialog.h \
    setaction.h \
    actionsettree.h \
    task.h \
    remotecontrol.h \
    remoteauthorizations.h \
    remotecontroltab.h \
    remoteauthtab.h \
    networkmanager.h \
    authconnecttab.h \
    common.h \
    networkmessage.h \
    actionreference.h \
    clientinfodialog.h \
    settingsregistry.h

FORMS += \
        mainwidget.ui \
    actiontab.ui \
    commandaction.ui \
    addtab.ui \
    actionitem.ui \
    addgroupdialog.ui \
    applicationaction.ui \
    groupconfigure.ui \
    linkaction.ui \
    logindialog.ui \
    mainpage.ui \
    settingsdialog.ui \
    setaction.ui \
    remotecontrol.ui \
    remoteauthorizations.ui \
    remotecontroltab.ui \
    remoteauthtab.ui \
    authconnecttab.ui \
    clientinfodialog.ui

RESOURCES += \
    resources.qrc

#include($$PWD/libs/singleapplication/src/qtsingleapplication.pri)
include($$PWD/lib/qtsingleapplication/src/qtsingleapplication.pri)

DISTFILES += \
    style.qss
