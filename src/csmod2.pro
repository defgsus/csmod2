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
    gui/patchgraphicsview.cpp \
    gui/cableitem.cpp \
    mod/module.cpp \
    mod/connector.cpp \
    mod/connection.cpp \
    mod/patch.cpp \
    gui/connectoritem.cpp \
    mod/model.cpp

HEADERS += \
    gui/mainwindow.h \
    gui/patchview.h \
    gui/moduleitem.h \
    gui/patchgraphicsview.h \
    gui/cableitem.h \
    mod/module.h \
    mod/base.h \
    mod/connector.h \
    mod/connection.h \
    mod/patch.h \
    gui/connectoritem.h \
    log.h \
    mod/model.h
