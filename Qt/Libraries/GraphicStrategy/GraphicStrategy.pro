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
INCLUDEPATH += ../WidgetToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/gpengine/include

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
    Actions/PrehistobotActions.cpp \
    Actions/WhileAction.cpp \
    StrategyMap.cpp \
    Strategies/TestStrategy.cpp \
    Strategies/StrategyEnumerator.cpp \
    Strategies/Ax12TestStrategy.cpp \
    StrategyInterface.cpp \
    Strategies/DefaultStrategy.cpp \
    Strategies/DefaultAIStrategy.cpp \
    Strategies/PrehistobotStrategy.cpp \
    Strategies/PrehistobotCommands.cpp \
    GameState.cpp \
    AbstractAICommand.cpp \
    AIEngine.cpp \
    Sensor.cpp \
    NSParser.cpp \
    NSRunner.cpp \
    NSParsingError.cpp \
    NSParser_BasicTypes.cpp


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
    Actions/PrehistobotActions.h  \
    Actions/WhileAction.h\
    StrategyMap.h \
    Strategies/TestStrategy.h \
    Strategies/StrategyEnumerator.h \
    Strategies/Ax12TestStrategy.h \
    Strategies/DefaultStrategy.h \
    Strategies/DefaultAIStrategy.h \
    Strategies/PrehistobotStrategy.h \
    Strategies/PrehistobotCommands.h \
    GameState.h \
    AbstractAICommand.h \
    AIEngine.h \
    Sensor.h \
    NSParser.h \
    ns.h \
    NSRunner.h \
    NSParsingError.h

FORMS += StrategyDialog.ui

RESOURCES += \
    GraphicStrategy.qrc


