#include "zoomimage.h"

#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

ZoomImage::ZoomImage(QWidget *parent):
    QLabel(parent),zoomRect(-1,-1,-1,-1)


{
    currentOperation = NONE;
    scaled = 1;
    this->setStyleSheet("border: 1px solid black");
}

void ZoomImage::setMainPixmap(QPixmap pixmap)
{
    mainPixmap = pixmap;

    currentRect = mainPixmap.rect();
    scaled = mainPixmap.width()/scaledTo();
    this->setPixmap(mainPixmap.scaledToWidth(scaledTo()));
}





void ZoomImage::paintEvent(QPaintEvent *event)
{


    QLabel::paintEvent(event);
    if (currentOperation == ZOOM)  {
        QPainter painter(this);
        painter.drawRect(zoomRect);
    }

}

void ZoomImage::mousePressEvent(QMouseEvent *event)
{

    if (currentOperation == ZOOM)  {
        zoomRect.setTopLeft(QPoint(event->x(),event->y())+pixmap()->rect().topLeft());
    }
    else if (currentOperation == MOVE) {
        startPoint=event->pos();
    }

}

void ZoomImage::mouseMoveEvent(QMouseEvent *event)
{

    if (currentOperation == ZOOM)  {
        zoomRect.setBottomRight(QPoint(event->x(),event->y()));
        repaint();
    }
    else if (currentOperation == MOVE) {




        setCursor(QCursor(Qt::ClosedHandCursor));
        QPoint newPoint = (startPoint-event->pos());
        if (newPoint.x()) newPoint.setX(abs(newPoint.x())/newPoint.x());
        if (newPoint.y()) newPoint.setY(abs(newPoint.y())/newPoint.y());

        startPoint=event->pos();

        QPoint newTopLeft = currentRect.topLeft()+newPoint;
        QPoint newRigthButton = currentRect.bottomRight()+newPoint;



        if ((newTopLeft.x()>0) && (newTopLeft.y()>0) && (newRigthButton.x()) < mainPixmap.width() &&
            (newRigthButton.y() < mainPixmap.height())) {

            currentRect.moveTo(currentRect.topLeft()+newPoint);
            move();
        }

    }


}



void ZoomImage::zoom()
{    
    if (zoomRect.width()<30) zoomRect.setWidth(30);
    if (zoomRect.height()<30) zoomRect.setHeight(30);
    currentRect.setTopLeft(currentRect.topLeft()+scaled*zoomRect.topLeft());
    currentRect.setBottomRight(currentRect.topLeft()+scaled*zoomRect.bottomRight()-scaled*zoomRect.topLeft());

    scaled *= zoomRect.width() / scaledTo();
    setPixmap(mainPixmap.copy(currentRect).scaledToWidth(scaledTo()));


    repaint();
    emit this->updateSize();
}

void ZoomImage::zoomOut()
{    
    currentRect =    mainPixmap.rect();
    scaled=mainPixmap.width()/scaledTo();
    setPixmap(mainPixmap.scaledToWidth(scaledTo()));

    emit this->updateSize();
}

void ZoomImage::move()
{
    setPixmap(mainPixmap.copy(currentRect).scaledToWidth(scaledTo()));
    this->setGeometry(this->pixmap()->rect());
    repaint();
}

double ZoomImage::scaledTo()
{
    qint32 preferedHeight, preferedWeight;
    QRect rec = QApplication::desktop()->screenGeometry();
    preferedHeight = rec.height()*0.4;
    preferedWeight = rec.width()*0.4;
    qint32 scaledTo=preferedWeight;

    double ImageRatio = (double)currentRect.width()/currentRect.height(); // соотношение размеров
    double temp = (double)preferedWeight / currentRect.width();


    //qDebug() << preferedHeight << " " << preferedWeight << " " << currentRect.height();
    if (currentRect.height()*temp > preferedHeight) scaledTo = preferedHeight*ImageRatio;

    return scaledTo;

}

void ZoomImage::ZoomInSetter()
{
    setCursor(QCursor(Qt::CrossCursor));
    currentOperation = ZOOM;
}

void ZoomImage::ZoomOutSetter()
{
    currentOperation = NONE;
    setCursor(QCursor(Qt::ArrowCursor));
    zoomOut();
}

void ZoomImage::MoveSetter()
{
    currentOperation = MOVE;
    setCursor(QCursor(Qt::OpenHandCursor));

}

void ZoomImage::mouseReleaseEvent(QMouseEvent *event)
{
    if (currentOperation == ZOOM)  {
        zoom();
        currentOperation = NONE;
        setCursor(QCursor(Qt::ArrowCursor));
        repaint();

    }
    else if (currentOperation == MOVE) {
        setCursor(QCursor(Qt::OpenHandCursor));
    }

}




