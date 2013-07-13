PROJECT                 = qextserialport
TEMPLATE                = lib

CONFIG                 += qt warn_on thread
#CONFIG                += dll
CONFIG                 += staticlib

# event driven device enumeration on windows requires the gui module
!win32:QT               -= gui

OBJECTS_DIR             = build/obj
MOC_DIR                 = build/moc

HEADERS                 = qextserialport.h \
                          qextserialenumerator.h
SOURCES                 = qextserialport.cpp \
                          qextserialenumerator.cpp

unix:SOURCES           += posix_qextserialport.cpp
macx: LIBS             += -framework IOKit

win32:SOURCES          += win_qextserialport.cpp
win32:DEFINES          += WINVER=0x0501 # needed for mingw to pull in appropriate dbt business...probably a better way to do this
unix:DEFINES += B1000000
win32:LIBS             += -lsetupapi


VERSION            = 1.2.0

DESTDIR = ../../ext-libs

CONFIG(debug, debug|release) {  
    TARGET = qextserialport-d
} else {
    TARGET = qextserialport
}
