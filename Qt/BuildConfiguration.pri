WINDOWS_COMPILER=mingw

#set dir variables
CONFIG(debug, debug|release) {
    LIBS_DIR = ../../../libs/debug
    EXT_DIR = ../../../ext-libs
    BUILD_DIR = build/debug
} else {
    LIBS_DIR = ../../../libs/release
    EXT_DIR = ../../../ext-libs
    BUILD_DIR = build/release
}

LIBS_HEADER_DIR = ../../Libraries
EXT_LIBS_HEADER_DIR = ../../../External

#set build dir
OBJECTS_DIR = $${BUILD_DIR}/obj
MOC_DIR = $${BUILD_DIR}/moc
RCC_DIR = $${BUILD_DIR}/rcc
UI_DIR = $${BUILD_DIR}/ui



CONFIG(staticlib) {
    #Libraries
    DESTDIR = $${LIBS_DIR}
    CONFIG(debug, debug|release) {
        TARGET = $${TARGET}-d
    }

} else {
    #Apps
    DESTDIR = ../../../bin
    CONFIG(debug, debug|release) {
        TARGET = $${TARGET}-d
    }

    LIBS      += -L$${LIBS_DIR}
    LIBS      += -L$${EXT_DIR}

    contains(WINDOWS_COMPILER,mingw) {
        LIB_EXT=a
    } else {
        win32: LIB_EXT=lib
        else:  LIB_EXT=a
    }

    #link with libs from this project
    for(dep, DEPENDENCY_LIBS) {  
        CONFIG(debug, debug|release) {
            LIB_NAME = $${dep}-d
        } else {
            LIB_NAME = $${dep}
        }

        contains(WINDOWS_COMPILER,mingw) {
           LIB_FILENAME = lib$${LIB_NAME}.$${LIB_EXT}
        } else {
            win32:LIB_FILENAME = $${LIB_NAME}.$${LIB_EXT}
            else:LIB_FILENAME = lib$${LIB_NAME}.$${LIB_EXT}
        }

        message($$TARGET depends on $$dep ($${DESTDIR}/$${LIB_FILENAME}))

        LIBS += -l$${LIB_NAME}
        PRE_TARGETDEPS += $${LIBS_DIR}/$${LIB_FILENAME}
    }

    #link with external libs
    for(dep, EXTERNAL_LIBS) {
        message($$TARGET linked with external lib $$dep ($${DESTDIR}/$${dep}.$${LIB_EXT}))
        CONFIG(debug, debug|release) {
            LIBS += -l$${dep}-d
        } else {
            LIBS += -l$${dep}
        }
    }
}
