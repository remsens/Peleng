#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:17:30
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    HyperCube/HyperCube.cpp \
    QCustomPlot/qcustomplot.cpp \
    HyperCube/GenericExc.cpp

HEADERS  += mainwindow.h \
    HyperCube/HyperCube.h \
    Interfaces/filereadinterface.h \
    Types.h \
    QCustomPlot/qcustomplot.h \
    HyperCube/GenericExc.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
