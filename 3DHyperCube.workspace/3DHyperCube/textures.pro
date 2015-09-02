HEADERS       = glwidget.h \
    qcustomplot.h \
    mainwindow.h \
    HyperCube.h \
    Types.h \
    gethypercube.h \
    ui_mainwindow.h \
    ui_mainwindowMy.h
SOURCES       = glwidget.cpp \
                main.cpp \
    qcustomplot.cpp \
    mainwindow.cpp \
    HyperCube.cpp \
    gethypercube.cpp



QT           += widgets
QT           += core gui printsupport

FORMS += \
    mainwindow.ui


CONFIG += c++11
