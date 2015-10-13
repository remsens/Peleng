#include "selectedline.h"
#include <QDebug>

SelectedLine::SelectedLine(QCustomPlot *parentPlot) : QCPItemLine(parentPlot), isSelected(false)
{

    start->setCoords(25000, 1e9);
    end->setCoords(25000, 0.001);
    QPen *pen = new QPen();
    pen->setWidth(10);

    this->setPen(*pen);
    //this->connect(this,SIGNAL(selectionChanged(bool)),this,SLOT(testSlot(bool)));


    //qDebug() << parentPlot->xAxis->range();
}

void SelectedLine::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{

    qDebug() << event;
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    qDebug() << this->parentPlot()->xAxis->pixelToCoord(mouseEvent->pos().x()) << this->parentPlot()->yAxis->pixelToCoord(mouseEvent->pos().y());
}

void SelectedLine::deselectEvent(bool *selectionStateChanged)
{
    qDebug() << selectionStateChanged;
}



/*void SelectedLine::testSlot(bool test)
{
    qDebug() << "Button pressed2";
}

void SelectedLine::mousePressEvent(QMouseEvent *event)
{
    isSelected = true;
    qDebug() << "Button pressed";
}

void SelectedLine::mouseReleaseEvent(QMouseEvent *event)
{
    isSelected = false;
    qDebug() << "Button released";
}

void SelectedLine::mouseMoveEvent(QMouseEvent *event)
{
     if (isSelected) qDebug("Ate key press %d", event->x());
}*/

