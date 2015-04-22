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
QTPLUGIN     += qjpeg qpng qbmp

SOURCES += main.cpp\
        mainwindow.cpp \
    localcontrastenhancer.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    localcontrastenhancer.h \
    utils.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
