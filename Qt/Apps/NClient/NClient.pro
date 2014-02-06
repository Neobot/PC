#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T15:55:36
#
#-------------------------------------------------

QT       += core gui network widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "NClient"
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_

DEPENDENCY_LIBS +=  NeobotGridEditor \
                    NeobotSimulation \
                    NeobotComm \
                    NeobotWidgetTools \
                    NeobotTools

EXTERNAL_LIBS +=    qextserialport \
                    QCustomPlot \
                    QtSolutions_PropertyBrowser

include( ../../BuildConfiguration.pri)

INCLUDEPATH += $${LIBS_HEADER_DIR}/GridEditorLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/CommLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/WidgetToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qextserialport/src
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/QCustomPlot
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qtpropertybrowser/src

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
    Ax12View.cpp

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
    Ax12View.h

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
    Ax12View.ui

RESOURCES += \
    NClient.qrc
