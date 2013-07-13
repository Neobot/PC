#-------------------------------------------------
#
# Project created by QtCreator 2011-03-30T20:37:22
#
#-------------------------------------------------

TARGET = NeobotSimulation
TEMPLATE = lib
CONFIG += staticlib

include( ../../BuildConfiguration.pri)

INCLUDEPATH += ../CommLib
INCLUDEPATH += ../ToolsLib

SOURCES += Simulator.cpp \
    SimRobot.cpp \
    SimulationMap.cpp \
    ChessUpObject.cpp \
    ChessUpConfiguration.cpp \
    SharpObject.cpp \
    SimProtocol.cpp \
    ContactorObject.cpp
HEADERS += Simulator.h \
    SimRobot.h \
    SimulationMap.h \
    ChessUpObject.h \
    ChessUpConfiguration.h \
    SharpObject.h \
    SimProtocol.h \
    ContactorObject.h
