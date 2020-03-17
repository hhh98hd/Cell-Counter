#-------------------------------------------------
#
# Project created by QtCreator 2019-12-27T21:53:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CellCounter
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    introwindow.cpp \
    browserdialog.cpp \
    watershedsegmenter.cpp \
    savedialog.cpp

HEADERS  += \
    mainwindow.h \
    introwindow.h \
    browserdialog.h \
    watershedsegmenter.h \
    savedialog.h

FORMS    += \
    mainwindow.ui \
    introwindow.ui \
    browserdialog.ui \
    savedialog.ui
QMAKE_CXXFLAGS += -std=c++11

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv
