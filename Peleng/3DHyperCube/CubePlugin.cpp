#include "CubePlugin.h"

#include <QSurfaceFormat>
#include <QtWidgets>

#include "GLWidget.h"
#include "mainwindow.h"

CubePlugin::CubePlugin()
{

}

CubePlugin::~CubePlugin()
{

}

void Execute(HyperCube* cube, IAttributes* attr)
{

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow mw;
    mw.processData(cube);// передаем в качестве параметра указатель на объект HyperCube
    mw.resize(1024,768);
    mw.show();
}
