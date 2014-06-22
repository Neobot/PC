
QT       -= core gui
CONFIG += c++11

CONFIG(debug, debug|release) {
    TARGET = gpengine-d
} else {
    TARGET = gpengine
}

TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../../ext-libs
OBJECTS_DIR = build/obj

INCLUDEPATH += include

HEADERS += include/ASTCreator.h \
    include/ASTNode.h \
    include/CGTFile.h \
    include/CharacterSetTable.h \
    include/DFA.h \
    include/DFAStateTable.h \
    include/ErrorReporter.h \
    include/ErrorTable.h \
    include/GrammarInfo.h \
    include/LALR.h \
    include/LALRStateTable.h \
    include/misc.h \
    include/NonTerminal.h \
    include/RuleTable.h \
    include/Symbol.h \
    include/SymbolStack.h \
    include/SymbolTable.h \
    include/Terminal.h \
    include/Token.h

SOURCES += src/ASTCreator.cpp \
    src/ASTNode.cpp \
    src/CGTFile.cpp \
    src/CharacterSetTable.cpp \
    src/DFA.cpp \
    src/DFAStateTable.cpp \
    src/ErrorReporter.cpp \
    src/ErrorTable.cpp \
    src/GrammarInfo.cpp \
    src/LALR.cpp \
    src/LALRStateTable.cpp \
    src/NonTerminal.cpp \
    src/RuleTable.cpp \
    src/Symbol.cpp \
    src/SymbolTable.cpp \
    src/Terminal.cpp \
    src/Token.cpp


