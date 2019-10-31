#-------------------------------------------------
#
# Project created by QtCreator 2017-08-15T05:07:06
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

#---------------------------------------------------------------------------------------------------
# Versión
#---------------------------------------------------------------------------------------------------
DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=0

#---------------------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
TARGET = ControlFlow_d
DEFINES += DEBUG
} else {
TARGET = ControlFlow
}

#---------------------------------------------------------------------------------------------------
# Directorio de destino
#---------------------------------------------------------------------------------------------------
DESTDIR = ../bin

#---------------------------------------------------------------------------------------------------
# Archivos de código
#---------------------------------------------------------------------------------------------------

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/Highlighter.cpp \
    src/Lexer.cpp \
    src/Input.cpp \
    src/ControlFlowCanvas.cpp \
    src/Parser.cpp \
    src/FlowNode.cpp \
    src/Token.cpp \
    src/FlowEdge.cpp \
    src/AboutDialog.cpp \
    src/utils.cpp \
    src/Languages.cpp

HEADERS  += src/MainWindow.h \
    src/Highlighter.h \
    src/Lexer.h \
    src/Input.h \
    src/ControlFlowCanvas.h \
    src/Parser.h \
    src/FlowNode.h \
    src/Token.h \
    src/FlowEdge.h \
    src/def_op.h \
    src/AboutDialog.h \
    src/utils.h \
    src/Languages.h

FORMS    += src/MainWindow.ui \
    src/AboutDialog.ui

#---------------------------------------------------------------------------------------------------
# Archivos de recursos
#---------------------------------------------------------------------------------------------------

RESOURCES += res/resource.qrc

win32:RC_FILE = res/resource_win32.rc

#---------------------------------------------------------------------------------------------------
# Archivos de traducción
#---------------------------------------------------------------------------------------------------
TRANSLATIONS =	translate/controlflow_es.ts \
				translate/controlflow_en.ts \
				translate/controlflow_it.ts
