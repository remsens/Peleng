#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T10:17:32
#
#-------------------------------------------------

QT       -= gui

TARGET = SpectralDistance
TEMPLATE = lib

DEFINES += SPECTRALDISTANCE_LIBRARY

SOURCES += spectraldistance.cpp

HEADERS += spectraldistance.h\
        spectraldistance_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
