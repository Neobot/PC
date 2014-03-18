# -------------------------------------------------
# Project created by QtCreator 2010-04-07T21:00:16
# -------------------------------------------------

TARGET = NeobotStrategy
TEMPLATE = lib
CONFIG += staticlib

QT += widgets
QT += serialport

include( ../../BuildConfiguration.pri)

INCLUDEPATH += ../CommLib
INCLUDEPATH += ../ToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather

win32 {
    INCLUDEPATH += ./strategies
    INCLUDEPATH += .
}


SOURCES += TrajectoryFinder.cpp \
    Sharp.cpp \
    SharpFactory.cpp \
    NMicropather.cpp \
    Pather.cpp \
    StrategyManager.cpp \
    ActionFactory.cpp \
    Actions/AbstractAction.cpp \
    Actions/StandardActions.cpp \
    Actions/ContainerActions.cpp \
    Actions/ConditionalActions.cpp \
    StrategyMap.cpp \
    Strategies/TestStrategy.cpp \
    Strategies/StrategyEnumerator.cpp \
    Strategies/Ax12TestStrategy.cpp \
    StrategyInterface.cpp \
    Strategies/DefaultStrategy.cpp \
    Strategies/DefaultAIStrategy.cpp \
    #Strategies/TreasureIslandStrategy.cpp \
    GameState.cpp \
    AbstractAICommand.cpp \
    Strategies/HappyBirthdayCommands.cpp \
    AIEngine.cpp \
    Strategies/HappyBirthdayStrategy.cpp \
    Sensor.cpp \
    ColorSensor.cpp

HEADERS += TrajectoryFinder.h \
    Sharp.h \
    StrategyInterface.h \
    SharpFactory.h \
    NMicropather.h \
    Pather.h \
    StrategyManager.h \
    ActionFactory.h \
    Actions/AbstractAction.h \
    Actions/StandardActions.h \
    Actions/ContainerActions.h \
    Actions/ConditionalActions.h \
    StrategyMap.h \
    Strategies/TestStrategy.h \
    Strategies/StrategyEnumerator.h \
    Strategies/Ax12TestStrategy.h \
    Strategies/DefaultStrategy.h \
    Strategies/DefaultAIStrategy.h \
    #Strategies/TreasureIslandStrategy.h \
    GameState.h \
    AbstractAICommand.h \
    Strategies/HappyBirthdayCommands.h \
    AIEngine.h \
    Strategies/HappyBirthdayStrategy.h \ 
    Sensor.h \
    ColorSensor.h

FORMS += StrategyDialog.ui


