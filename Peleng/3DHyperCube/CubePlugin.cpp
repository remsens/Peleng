#include "CubePlugin.h"
#include <QSurfaceFormat>
#include "mainwindow.h"
#include <QDebug>
#include "../SpectrPlotter/PlotterWindow.h"
#include "../LinePlotter/LinePlotterWindow.h"
CubePlugin::CubePlugin(QObject *parent)
{
   // m_w = new MainWindow();
}

CubePlugin::~CubePlugin()
{
    //delete m_w;
}

QObject* CubePlugin::GetObjectPointer()
{
    return this;
}

void CubePlugin::Execute(HyperCube* cube, Attributes *attr)
{
    // в данном плагине список точек нам не нужен. Но для общего интерфейса мы их передаем
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

    QCoreApplication::processEvents();
    m_w = new MainWindow();
    m_w->processData(cube, attr);// передаем в качестве параметра указатель на объект HyperCube
    m_w->resize(1024,768);
    labelSplash->hide();
    m_w->show();
    delete labelSplash;
    delete pix;

}
