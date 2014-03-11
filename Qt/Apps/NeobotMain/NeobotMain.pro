# -------------------------------------------------
# Project created by QtCreator 2010-01-16T15:07:17
# -------------------------------------------------

TARGET = NeobotMain
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

QT += widgets
QT += serialport

DEPENDENCY_LIBS +=  NeobotStrategy \
                    NeobotSimulation \
                    NeobotComm \
                    NeobotTools

EXTERNAL_LIBS +=    Micropather

include( ../../BuildConfiguration.pri)

INCLUDEPATH += $${LIBS_HEADER_DIR}/GraphicsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/GraphicStrategy
INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/Simulation
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather

SOURCES += main.cpp \
    NeobotMainOptions.cpp \
    NeobotStrategyLauncher.cpp

HEADERS += NeobotMainOptions.h \
    NeobotStrategyLauncher.h
