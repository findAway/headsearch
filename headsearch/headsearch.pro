#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T22:04:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = headsearch
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    projectanalyse.cpp

HEADERS  += mainwidget.h \
    ui_mainwidget.h \
    projectanalyse.h

FORMS    += mainwidget.ui

RESOURCES += \
    icon.qrc

RC_FILE += icon.rc

DISTFILES += \
    icon.rc
