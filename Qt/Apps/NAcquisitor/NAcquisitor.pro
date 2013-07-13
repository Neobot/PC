#-------------------------------------------------
#
# Project created by QtCreator 2012-05-24T19:30:07
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NAcquisitor
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_

DEPENDENCY_LIBS +=  NeobotSimulation \
                    NeobotComm \
                    NeobotTools \
                    NeobotWidgetTools

EXTERNAL_LIBS +=    qextserialport \
                    QCustomPlot

include( ../../BuildConfiguration.pri)

INCLUDEPATH += $${LIBS_HEADER_DIR}/Simulation
INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/WidgetToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qextserialport
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/QCustomPlot
win32 {
    INCLUDEPATH += ./Views
    INCLUDEPATH += .
}

macx:LIBS += -framework IOKit -framework CoreFoundation
win32:LIBS += -lsetupapi -ladvapi32 -luser32

SOURCES += main.cpp\
        NAcquisitor.cpp \
    ViewWidget.cpp \
    Views/RawDataView.cpp \
    Views/CommandView.cpp \
    Views/SimpleCurveView.cpp \
    Views/DoubleCurveView.cpp \
    Acquisition.cpp \
    Samples.cpp \
    Views/LogView.cpp \
    Views/SharpView.cpp

HEADERS  += NAcquisitor.h \
    ViewWidget.h \
    Views/RawDataView.h \
    Views/CommandView.h \
    Views/SimpleCurveView.h \
    ViewInterface.h \
    Views/DoubleCurveView.h \
    Acquisition.h \
    Samples.h \
    Views/LogView.h \
    Views/SharpView.h

FORMS    += NAcquisitor.ui \
    ViewWidget.ui \
    Views/RawDataView.ui \
    Views/CommandView.ui \
    Views/LogView.ui

RESOURCES += \
    NAcquisitor.qrc
