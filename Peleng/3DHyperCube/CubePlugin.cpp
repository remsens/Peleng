#include "CubePlugin.h"
#include <QSurfaceFormat>
#include "mainwindow.h"

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

    MainWindow mw;
    mw.processData(cube);// передаем в качестве параметра указатель на объект HyperCube
    mw.resize(1024,768);
    mw.show();
}
