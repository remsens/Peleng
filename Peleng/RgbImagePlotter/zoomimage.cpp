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
        zoomRect.setTopLeft(QPoint(event->x(),event->y())/*+pixmap()->rect().topLeft()*/);
    }
    else if (currentOperation == MOVE)  {
        startPoint = event->pos();
        startLeft = currentRect.topLeft();
    }
}

void ZoomImage::mouseMoveEvent(QMouseEvent *event)
{

    if (currentOperation == ZOOM)  {
        zoomRect.setBottomRight(QPoint(event->x(),event->y())/*+pixmap()->rect().topLeft()*/);
        repaint();
    }
    else if (currentOperation == MOVE) {
        setCursor(QCursor(Qt::ClosedHandCursor));
        QPoint newPoint = (startPoint-event->pos())*scaled;

        QRect temp;
        temp.setTopLeft(currentRect.topLeft()+scaled*zoomRect.topLeft());
        temp.setBottomRight(currentRect.topLeft()+scaled*zoomRect.bottomRight());
      //  qDebug() << temp.bottom() << " " << temp.right() << " " << temp.top() << " " << temp.left();
       /* if ((temp.left() > 50) && (temp.right() < mainPixmap.width()-50) &&
            (temp.top() > 50) && (temp.bottom() < mainPixmap.height()-50)){*/
            currentRect.moveTo(startLeft+newPoint);
            move();
        //}
    }


}



void ZoomImage::zoom()
{

    currentRect.setTopLeft(currentRect.topLeft()+scaled*zoomRect.topLeft());
    currentRect.setBottomRight(currentRect.topLeft()+scaled*zoomRect.bottomRight());
    scaled *= zoomRect.width() / scaledTo();
    setPixmap(mainPixmap.copy(currentRect).scaledToWidth(scaledTo()));

    //qDebug() << currentRect;
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
    preferedHeight = rec.height()*0.6;
    preferedWeight = rec.width()*0.6;
    qint32 scaledTo=preferedWeight;
    if (currentRect.height() >preferedWeight) scaledTo = preferedHeight;

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




