TEMPLATE=lib
CONFIG += qt staticlib qtpropertybrowser-buildlib
mac:CONFIG += absolute_library_soname

include(../src/qtpropertybrowser.pri)
TARGET = $$QTPROPERTYBROWSER_LIBNAME
DESTDIR = $$QTPROPERTYBROWSER_LIBDIR
win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QTPROPERTYBROWSER_LIBNAME}.dll
}
target.path = $$DESTDIR
INSTALLS += target
