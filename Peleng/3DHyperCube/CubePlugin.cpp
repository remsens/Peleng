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
    for (int i = 0; i < m_listWindows.size(); i++)
    {
        delete m_listWindows.at(i);
        m_listWindows.removeAt(i);
    }
}

void CubePlugin::OnClose(MainWindow* window)
{
    for (int i = 0; i < m_listWindows.size(); i++)
    {
        if (m_listWindows.at(i) == window)
        {
            m_listWindows.removeAt(i);
        }
    }
}

void CubePlugin::onStart(bool flag)
{
    emit StartOperation(flag);
}

void CubePlugin::onFinish(bool flag)
{
    emit FinishOperation(flag);
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
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    QPixmap*pixtemp = new QPixmap(":/Splash/iconsCube/cube9.png");
    QPixmap* pix = new QPixmap(pixtemp->scaledToHeight(screenGeometry.height()*0.8));
    //QMovie *movie = new QMovie(":/Splash/iconsCube/add-loading-screen.gif");
    QLabel* labelSplash = new QLabel();

    labelSplash->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    labelSplash->setPixmap(*pix);
    labelSplash->setProperty("windowOpacity",0.8);

    int x = (screenGeometry.width()-pix->width()) / 2;
    int y = (screenGeometry.height()-pix->height()) / 2;
    labelSplash->move(x,y);
    //labelSplash->setMovie(movie);
   // movie->start();
    labelSplash->show();

    QCoreApplication::processEvents();
    MainWindow* w = new MainWindow(cube, attr);
    QObject::connect(w, SIGNAL(Close(MainWindow*)), this, SLOT(OnClose(MainWindow*)));
    QObject::connect(w,SIGNAL(StartOperation(bool)),this,SLOT(onStart(bool)));
    QObject::connect(w,SIGNAL(FinishOperation(bool)),this,SLOT(onFinish(bool)));
    m_listWindows.append(w);
    w->processData();
    w->resize(screenGeometry.width()*0.6,screenGeometry.height()*0.8);
    labelSplash->hide();
    w->show();

    delete labelSplash;
    delete pix;

}
