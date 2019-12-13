#-------------------------------------------------
#
# Project created by QtCreator 2014-03-29T15:11:54
#
#-------------------------------------------------

QT       += core gui

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
DESTDIR = bin


greaterThan(QT_MAJOR_VERSION, 4) {
    LIBS += -lQt5SerialPort
} else {
    LIBS += -lqtserialport
}

include(../../DCONLib.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DCONTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp
HEADERS  += mainwindow.h
FORMS    += mainwindow.ui


include(../../../qtserialportwidgets/qtserialportwidgets.pri)
