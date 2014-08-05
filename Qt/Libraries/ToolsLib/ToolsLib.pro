# -------------------------------------------------
# Project created by QtCreator 2010-02-23T19:00:38
# -------------------------------------------------

TARGET = NeobotTools
TEMPLATE = lib
CONFIG += staticlib

QT += widgets

include( ../../BuildConfiguration.pri)

HEADERS += ToolsLib.h \
    AbstractLogger.h \
    Point.h \
    Curve.h \
    vmath.h \
    RPoint.h \
    RMovement.h \
    DataMap.h \
    DataObject.h \
    ZRange.h \
    NGrid.h \
    NGridArea.h \
    NSettings.h \
    Ax12MovementManager.h \
    SignalDelayer.h

SOURCES += ToolsLib.cpp \
    AbstractLogger.cpp \
    Point.cpp \
    Curve.cpp \
    RPoint.cpp \
    RMovement.cpp \
    DataMap.cpp \
    DataObject.cpp \
    ZRange.cpp \
    NGrid.cpp \
    NGridArea.cpp \
    NSettings.cpp \
    Ax12MovementManager.cpp \
    SignalDelayer.cpp
