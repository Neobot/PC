# -------------------------------------------------
# Project created by QtCreator 2010-01-16T15:02:29
# -------------------------------------------------

TARGET = NeobotComm
TEMPLATE = lib
CONFIG += staticlib

include( ../../BuildConfiguration.pri)

INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qextserialport
INCLUDEPATH += ../ToolsLib

unix:DEFINES = _TTY_POSIX_
win32:DEFINES = _TTY_WIN_
SOURCES += RobotProtocol.cpp \
    Data.cpp \
    RobotCommInterface.cpp \
    CommException.cpp \
    AbstractProtocol.cpp \
    AbstractCommInterface.cpp \
    ProtocolAX12.cpp \
    ProtocolRaw.cpp \
    AX12CommManager.cpp \
    CommDispatcher.cpp \
    Instructions.cpp \
    ProtocolUM6.cpp \
    UM6.cpp \
    NetworkProtocol.cpp \
    Ax12MovementRunner.cpp \
    AX12StatusListener.cpp
HEADERS += RobotProtocol.h \
    Data.h \
    Instructions.h \
    RobotCommInterface.h \
    CommException.h \
    CommListener.h \
    AbstractProtocol.h \
    AbstractCommInterface.h \
    ProtocolAX12.h \
    ProtocolRaw.h \
    AX12CommManager.h \
    CommDispatcher.h \
    ProtocolUM6.h \
    UM6.h \
    NetworkProtocol.h \
    Ax12MovementRunner.h \
    AX12StatusListener.h

OTHER_FILES +=
