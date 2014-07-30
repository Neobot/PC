#-------------------------------------------------
#
# Project created by QtCreator 2012-11-18T00:01:08
#
#-------------------------------------------------

TARGET = NServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

QT       += core
QT       += network
QT       += widgets
QT	 += serialport

DEPENDENCY_LIBS +=  NeobotStrategy \
                    NeobotSimulation \
                    NeobotComm \
                    NeobotTools

EXTERNAL_LIBS +=   Micropather \
		   gpengine

include( ../../BuildConfiguration.pri)

INCLUDEPATH += $${LIBS_HEADER_DIR}/GraphicStrategy
INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/Simulation
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather

macx:LIBS += -framework IOKit -framework CoreFoundation
win32:LIBS += -lsetupapi -ladvapi32 -luser32

SOURCES += main.cpp \
    NServer.cpp \
    NetworkCommInterface.cpp \
    ServerAX12RequestManager.cpp

HEADERS += \
    NServer.h \
    NetworkCommInterface.h \
    NetworkCommListener.h \
    ServerAX12RequestManager.h

