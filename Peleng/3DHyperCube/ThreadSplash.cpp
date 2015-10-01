#include "ThreadSplash.h"

ThreadSplash::ThreadSplash()
{
    //moveToThread(this);
}

ThreadSplash::~ThreadSplash()
{

}

void ThreadSplash::run()
{

    QPixmap *pix = new QPixmap(":/Splash/iconsCube/CubeSplashScreen.jpg");
    labelSplash = new QLabel();
    labelSplash->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    labelSplash->setPixmap(*pix);
    labelSplash->setMask(pix->mask());
    labelSplash->show();
}

void ThreadSplash::slotClose()
{

    labelSplash->hide();
    exit();
}

