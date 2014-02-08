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
    mod/model.cpp \
    mod/modulestock.cpp

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
    mod/model.h \
    mod/modulestock.h

######### MODULES ########

MODULE_SOURCES += \
    module/math.cpp

MODULE_HEADERS += \
    module/math.h

#OTHER_FILES += $$MODULE_SOURCES $$MODULE_HEADERS
SOURCES += $$MODULE_SOURCES
HEADERS += $$MODULE_HEADERS

###### module compiler ######

modules.input = MODULE_SOURCES
modules.output = ${QMAKE_FILE_BASE}.o
modules.commands = $$QMAKE_CXX $$QMAKE_CXXFLAGS $$LIBS \
                    $$INCLUDEPATH $$QMAKE_LIBS \
                    -I/home/defgsus/prog/qt_project/csmod2/src \
                    -o ${QMAKE_FILE_BASE} ${QMAKE_FILE_NAME}

#QMAKE_EXTRA_COMPILERS += modules
