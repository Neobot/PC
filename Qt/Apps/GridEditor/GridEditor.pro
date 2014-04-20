#-------------------------------------------------
#
# Project created by QtCreator 2011-09-30T20:51:43
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = GridEditor
TEMPLATE = app

CONFIG += console
#CONFIG -= app_bundle
TEMPLATE = app

DEPENDENCY_LIBS += NeobotGridEditor \
                   NeobotTools


include( ../../BuildConfiguration.pri )

INCLUDEPATH += $${LIBS_HEADER_DIR}/ToolsLib
INCLUDEPATH += $${LIBS_HEADER_DIR}/GridEditorLib


SOURCES += main.cpp





















