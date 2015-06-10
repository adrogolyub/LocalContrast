#-------------------------------------------------
#
# Project created by QtCreator 2015-04-22T20:15:30
#
#-------------------------------------------------

QT       = core gui

include(opencv.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


message($$LIBS))
TARGET = LocalContrast
TEMPLATE = app
QTPLUGIN     += qpng

SOURCES += main.cpp\
        mainwindow.cpp \
    localcontrastenhancer.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    localcontrastenhancer.h \
    utils.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    resource.qrc
OTHER_FILES += app.rc
RC_FILE = app.rc
