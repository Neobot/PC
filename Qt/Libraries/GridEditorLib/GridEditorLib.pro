#-------------------------------------------------
#
# Project created by QtCreator 2013-02-24T19:22:14
#
#-------------------------------------------------

TARGET = NeobotGridEditor
TEMPLATE = lib
CONFIG += staticlib

QT += widgets

include( ../../BuildConfiguration.pri)

INCLUDEPATH += ../ToolsLib

win32 {
    INCLUDEPATH += ./Inspectors
    INCLUDEPATH += .
}

SOURCES += GridEditor.cpp \
    GridScene.cpp \
    Commands.cpp \
    LinkItem.cpp \
    SceneTools.cpp \
    GridView.cpp \
    NodeItem.cpp \
    LinkedNodeEditor.cpp \
    ArrowWidget.cpp \
    NewGridDialog.cpp \
    AreaItem.cpp \
    Inspectors/Inspector.cpp \
    Inspectors/AreaInspector.cpp \
    Inspectors/NodeInspector.cpp

HEADERS  += GridEditor.h \
    GridScene.h \
    Commands.h \
    LinkItem.h \
    SceneTools.h \
    GridView.h \
    NodeItem.h \
    LinkedNodeEditor.h \
    ArrowWidget.h \
    NewGridDialog.h \
    AreaItem.h \
    Inspectors/Inspector.h \
    Inspectors/AreaInspector.h \
    Inspectors/NodeInspector.h

FORMS    += GridEditor.ui \
    LinkedNodeEditor.ui \
    NewGridDialog.ui \
    Inspectors/Inspector.ui \
    Inspectors/AreaInspector.ui \
    Inspectors/NodeInspector.ui

RESOURCES +=  GridEditor.qrc \
    ../WidgetToolsLib/WidgetToolsLib.qrc
