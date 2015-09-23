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

    MainWindow* mw = new MainWindow();

    mw->processData(cube);// передаем в качестве параметра указатель на объект HyperCube
    mw->resize(1024,768);
    mw->show();
}
