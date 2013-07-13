exists(config.pri):infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtpropertybrowser-uselib
TEMPLATE += fakelib
CONFIG(debug, debug|release) {
    QTPROPERTYBROWSER_LIBNAME = QtSolutions_PropertyBrowser-d
} else {
    QTPROPERTYBROWSER_LIBNAME = QtSolutions_PropertyBrowser
}
TEMPLATE -= fakelib
QTPROPERTYBROWSER_LIBDIR = ../../../ext-libs
unix:qtpropertybrowser-uselib:!qtpropertybrowser-buildlib:QMAKE_RPATHDIR += $$QTPROPERTYBROWSER_LIBDIR
