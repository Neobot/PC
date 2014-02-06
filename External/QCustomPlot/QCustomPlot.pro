#-------------------------------------------------
#
# Project created by QtCreator 2012-08-04T18:38:21
#
#-------------------------------------------------

QT += widgets
QT += printsupport

CONFIG(debug, debug|release) {
    TARGET = QCustomPlot-d
} else {
    TARGET = QCustomPlot
}

TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../../ext-libs
OBJECTS_DIR = build/obj

SOURCES += QCustomPlot.cpp
HEADERS += QCustomPlot.h

