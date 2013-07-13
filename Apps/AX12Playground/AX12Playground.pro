# -------------------------------------------------
# Project created by QtCreator 2010-01-10T14:17:22
# -------------------------------------------------
TARGET = AX12Playground
CONFIG += console
macx:CONFIG -= app_bundle
TEMPLATE = app

DEPENDENCY_LIBS +=  NeobotComm \
                    NeobotTools

EXTERNAL_LIBS +=    qextserialport

include( ../../BuildConfiguration.pri )

INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qextserialport
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/RoBoIO/Include

unix:DEFINES = _TTY_POSIX_
win32:DEFINES = _TTY_WIN_

SOURCES += main.cpp

#discard compilation on windows
#TODO: refine this behavior
unix {
HEADERS += AX12Playground.h \
    ThreeAxisArm.h \
    Hexapod.h

SOURCES += AX12Playground.cpp \
    ThreeAxisArm.cpp \
    Hexapod.cpp

FORMS += \
    AX12Playground.ui
}

