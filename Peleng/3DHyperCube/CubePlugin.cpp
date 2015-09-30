#include "CubePlugin.h"
#include <QSurfaceFormat>
#include "mainwindow.h"
#include <QDebug>
#include "../SpectrPlotter/PlotterWindow.h"

CubePlugin::CubePlugin(QObject *parent)
{

}

CubePlugin::~CubePlugin()
{

}

void CubePlugin::Execute(HyperCube* cube, IAttributes* attr)
{

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    qDebug() << "Зашли в плагин";
    //    QSplashScreen *splash = new QSplashScreen(QPixmap(":/Splash/iconsCube/CubeSplashScreen.jpg"));
    //    splash->show();
    //    splash->raise();
    QMainWindow* windSplash = new QMainWindow();
    QPixmap*pix = new QPixmap(":/Splash/iconsCube/CubeSplashScreen.jpg");
    QLabel* labelSplash = new QLabel();
    //labelSplash->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    labelSplash->setPixmap(*pix);
    labelSplash->setMask(pix->mask());
    windSplash->setCentralWidget(labelSplash);
    windSplash->show();
    MainWindow* mw = new MainWindow();
    mw->processData(cube);// передаем в качестве параметра указатель на объект HyperCube
    mw->resize(1024,768);
    windSplash->hide();
    mw->show();
    delete labelSplash;

}
