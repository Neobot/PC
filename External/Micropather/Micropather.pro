
QT       -= core gui
CONFIG += c++11

CONFIG(debug, debug|release) {
    TARGET = Micropather-d
} else {
    TARGET = Micropather
}

TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../../ext-libs
OBJECTS_DIR = build/obj

HEADERS += \
    micropather.h

SOURCES += \
    micropather.cpp


