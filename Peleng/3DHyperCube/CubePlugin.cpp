#include "CubePlugin.h"
#include <QSurfaceFormat>
#include "mainwindow.h"
#include <QDebug>
#include "../SpectrPlotter/PlotterWindow.h"
#include "../LinePlotter/LinePlotterWindow.h"
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

    QPixmap*pixtemp = new QPixmap(":/Splash/iconsCube/cube9.png");
    QPixmap* pix = new QPixmap(pixtemp->scaledToHeight(800));
    //QMovie *movie = new QMovie(":/Splash/iconsCube/add-loading-screen.gif");
    QLabel* labelSplash = new QLabel();

    labelSplash->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    labelSplash->setPixmap(*pix);
    labelSplash->setProperty("windowOpacity",0.8);
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-pix->width()) / 2;
    int y = (screenGeometry.height()-pix->height()) / 2;
    labelSplash->move(x,y);
    //labelSplash->setMovie(movie);
   // movie->start();
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
