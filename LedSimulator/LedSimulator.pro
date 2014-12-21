#-------------------------------------------------
#
# Project created by QtCreator 2014-11-21T17:49:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LedSimulator
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    renderarea.cpp \
    ledLineEdit.cpp \
    MemoryMonitor.cpp

HEADERS  += MainWindow.h \
    renderarea.h \
    ledLineEdit.h \
    MemoryMonitor.h

FORMS    += MainWindow.ui
QMAKE_MAC_SDK = macosx10.10

