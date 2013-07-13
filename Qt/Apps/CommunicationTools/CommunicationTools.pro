# -------------------------------------------------
# Project created by QtCreator 2010-01-10T14:17:22
# -------------------------------------------------
TARGET = nct
CONFIG += console
macx:CONFIG -= app_bundle
TEMPLATE = app

DEPENDENCY_LIBS +=  NeobotComm \
                    NeobotWidgetTools \
                    NeobotTools

EXTERNAL_LIBS +=    qextserialport

include( ../../BuildConfiguration.pri )

INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/WidgetToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qextserialport

macx:LIBS += -framework IOKit -framework CoreFoundation
win32:LIBS += -lsetupapi -ladvapi32 -luser32


unix:DEFINES = _TTY_POSIX_
win32:DEFINES = _TTY_WIN_
DEFINES += B1000000

HEADERS += CommTools.h \
    CommGui.h \
    CommUtil.h \
    UtilitySelector.h \
    AX12Utilities.h
FORMS += CommGui.ui \
    UtilitySelector.ui \
    AX12Utilities.ui
SOURCES += main.cpp \
    CommTools.cpp \
    CommGui.cpp \
    CommUtil.cpp \
    UtilitySelector.cpp \
    AX12Utilities.cpp
