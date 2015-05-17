#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T15:55:36
#
#-------------------------------------------------

QT       += core gui network widgets printsupport serialport webkitwidgets

TARGET = "NHexClient"
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
		NHexClient.cpp \
    ConnectionView.cpp \
	AbstractView.cpp \
    CommInterface.cpp

HEADERS  += NHexClient.h \
    ConnectionView.h \
	AbstractView.h \
    CommInterface.h

FORMS    += NHexClient.ui \
	ConnectionView.ui

RESOURCES += \
	NHexClient.qrc
