# -------------------------------------------------
# Project created by QtCreator 2010-04-07T21:00:16
# -------------------------------------------------

TARGET = NeobotStrategy
TEMPLATE = lib
CONFIG += staticlib

QT += widgets

include( ../../BuildConfiguration.pri)

INCLUDEPATH += ../CommLib
INCLUDEPATH += ../ToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qextserialport/src

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
    AbstractAction.cpp \
    ActionFactory.cpp \
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
    StandardActions.cpp
HEADERS += TrajectoryFinder.h \
    Sharp.h \
    StrategyInterface.h \
    SharpFactory.h \
    NMicropather.h \
    Pather.h \
    StrategyManager.h \
    AbstractAction.h \
    ActionFactory.h \
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
    StandardActions.h
FORMS += StrategyDialog.ui


