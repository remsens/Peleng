HEADERS       = \
    HyperCube.h \
    Types.h \
    ui_mainwindow.h \
    GetHyperCube.h \
    MainWindow.h \
    QCustomPlot.h \
    GLWidget.h \
    ui_MainWindow3DCube.h
SOURCES       = \
    HyperCube.cpp \
    GetHyperCube.cpp \
    GLWidget.cpp \
    Main.cpp \
    MainWindow.cpp \
    QCustomPlot.cpp



QT           += widgets
QT           += core gui printsupport

FORMS += \
    mainwindow.ui


CONFIG += c++11
