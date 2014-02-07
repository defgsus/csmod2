#-------------------------------------------------
#
# Project created by QtCreator 2014-02-06T22:32:06
#
#-------------------------------------------------

TARGET = csmod2

####### QT STUFF #######

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

######### FLAGS ########

QMAKE_CXXFLAGS = --std=c++0x

######## FILES #########

SOURCES += \
    main.cpp \
    gui/mainwindow.cpp \
    gui/patchview.cpp \
    gui/moduleitem.cpp \
    gui/patchgraphicsview.cpp

HEADERS += \
    gui/mainwindow.h \
    gui/patchview.h \
    gui/moduleitem.h \
    gui/patchgraphicsview.h
