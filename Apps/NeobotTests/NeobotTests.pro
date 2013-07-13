# -------------------------------------------------
# Project created by QtCreator 2010-02-14T13:23:38
# -------------------------------------------------
QT += testlib
TARGET = NeobotTests
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEPENDENCY_LIBS +=  NeobotStrategy \
                    NeobotSimulation \
                    NeobotComm \
                    NeobotTools

EXTERNAL_LIBS +=    qextserialport \
                    Micropather

include( ../../BuildConfiguration.pri)

INCLUDEPATH += $${LIBS_HEADER_DIR}/GraphicStrategy
INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/Micropather
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qextserialport

SOURCES += main.cpp \
    DataTests.cpp \
    PointTests.cpp \
    CurveTests.cpp \
    ProtocolTests.cpp \
    GraphicStrategyTest.cpp \
    PatherTests.cpp \
    ZRangeTests.cpp \
    DataMapTests.cpp \
    NGridTests.cpp \
    NSettingsTests.cpp \
    Ax12MovementManagerTest.cpp

HEADERS += DataTests.h \
    PointTests.h \
    CurveTests.h \
    ProtocolTests.h \
    GraphicStrategyTest.h \
    PatherTests.h \
    ZRangeTests.h \
    DataMapTests.h \
    NGridTests.h \
    NSettingsTests.h \
    Ax12MovementManagerTest.h

