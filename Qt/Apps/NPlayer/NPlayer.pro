#-------------------------------------------------
#
# Project created by QtCreator 2011-04-30T18:28:49
#
#-------------------------------------------------

QT       += core gui widgets serialport

TARGET = NPlayer
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEPENDENCY_LIBS +=  NeobotStrategy \
                    NeobotSimulation \
                    NeobotComm \
                    NeobotWidgetTools \
                    NeobotTools

EXTERNAL_LIBS +=    Micropather \
		    gpengine

include( ../../BuildConfiguration.pri)

win32:INCLUDEPATH += .
INCLUDEPATH += $${LIBS_HEADER_DIR}/GraphicStrategy
INCLUDEPATH += $${LIBS_HEADER_DIR}/Simulation
INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/WidgetToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather

SOURCES += main.cpp\
        NPlayer.cpp \
    ChessUpConfigCardWidget.cpp \
    ChessUpConfigurationSelector.cpp

HEADERS  += NPlayer.h \
    ChessUpConfigCardWidget.h \
    ChessUpConfigurationSelector.h

FORMS    += NPlayer.ui \
    ChessUpConfigCardWidget.ui \
    ChessUpConfigurationSelector.ui

RESOURCES += \
    NPlayer.qrc
