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

    QPixmap*pix = new QPixmap(":/Splash/iconsCube/the_gentlemanly_escort_cube.png");
    QLabel* labelSplash = new QLabel();
    labelSplash->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    labelSplash->setPixmap(*pix);
    //labelSplash->setMask(pix->mask());
    labelSplash->show();
    MainWindow* mw = new MainWindow();
    QCoreApplication::processEvents();
    mw->processData(cube);// передаем в качестве параметра указатель на объект HyperCube
    mw->resize(1024,768);
    labelSplash->hide();
    mw->show();
    delete labelSplash;
    delete pix;

}
