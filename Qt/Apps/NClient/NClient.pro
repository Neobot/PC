#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T15:55:36
#
#-------------------------------------------------

QT       += core gui network widgets printsupport serialport

TARGET = "NClient"
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_

DEPENDENCY_LIBS +=  NeobotGridEditor \
		    NeobotStrategy \
                    NeobotSimulation \
                    NeobotComm \
                    NeobotWidgetTools \
                    NeobotTools

EXTERNAL_LIBS +=    QCustomPlot \
		    QtSolutions_PropertyBrowser \
		    gpengine \
		    Micropather

include(../../BuildConfiguration.pri)

INCLUDEPATH += $${LIBS_HEADER_DIR}/GridEditorLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/GraphicStrategy
INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/WidgetToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/QCustomPlot
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qtpropertybrowser/src
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/gpengine/include

SOURCES += main.cpp\
        NClient.cpp \
    NetworkClientCommInterface.cpp \
    NetworkConnection.cpp \
    ConnectionView.cpp \
    AbstractView.cpp \
    LogView.cpp \
    RemoteControlView.cpp \
    TeleportDialog.cpp \
    OdometryView.cpp \
    SensorsView.cpp \
    StoredTrajectoriesManagerDialog.cpp \
    StrategiesView.cpp \
    ParametersView.cpp \
    Ax12View.cpp \
    GraphsView.cpp \
    ScriptView.cpp

HEADERS  += NClient.h \
    NetworkClientCommInterface.h \
    NetworkClientCommListener.h \
    NetworkConnection.h \
    ConnectionView.h \
    AbstractView.h \
    LogView.h \
    RemoteControlView.h \
    TeleportDialog.h \
    OdometryView.h \
    SensorsView.h \
    StoredTrajectoriesManagerDialog.h \
    StrategiesView.h \
    ParametersView.h \
    Ax12View.h \
    GraphsView.h \
    ScriptView.h

FORMS    += NClient.ui \
    ConnectionView.ui \
    LogView.ui \
    RemoteControlView.ui \
    TeleportDialog.ui \
    OdometryView.ui \
    SensorsView.ui \
    StoredTrajectoriesManagerDialog.ui \
    StrategiesView.ui \
    ParametersView.ui \
    Ax12View.ui \
    GraphsView.ui \
    ScriptView.ui

RESOURCES += \
    NClient.qrc
