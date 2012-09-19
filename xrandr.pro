#-------------------------------------------------
#
# Project created by QtCreator 2012-06-17T12:54:20
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xrandr
TEMPLATE = app


CONFIG += link_pkgconfig
PKGCONFIG += libnotify


SOURCES += main.cpp\
        mainwindow.cpp \
    qactionkrida.cpp \
    xmlparser.cpp \
    popup.cpp \
    popup_neco.cpp \
    qtextbrowserkrida.cpp

HEADERS  += mainwindow.h \
    qactionkrida.h \
    xmlparser.h \
    popup.h \
    popup_neco.h \
    qtextbrowserkrida.h

FORMS += \
    popup_neco.ui
