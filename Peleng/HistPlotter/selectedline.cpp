#include "selectedline.h"
#include <QDebug>

SelectedLine::SelectedLine(QCustomPlot *parentPlot) : QCPItemLine(parentPlot), isSelected(false)
{

    start->setCoords(25000, 1e9);
    end->setCoords(25000, 0.001);
    QPen *pen = new QPen();
    pen->setWidth(10);

    this->setPen(*pen);
    this->connect(this,SIGNAL(selectionChanged(bool)),SLOT(testSlot(bool)));


    //qDebug() << parentPlot->xAxis->range();
}

void SelectedLine::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    qDebug() << "Button pressed";
}

void SelectedLine::testSlot(bool test)
{
    qDebug() << "Button pressed2";
}

/*void SelectedLine::mousePressEvent(QMouseEvent *event)
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

