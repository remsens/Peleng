#include "histplugin.h"


#include <QDebug>





HistPlugin::HistPlugin(QObject *parent) : QObject(parent)
{
    customPlot =  new QCustomPlot();
}

void HistPlugin::plotLine(QCPAbstractPlottable *object, QMouseEvent *event)
{
       double x = object->parentPlot()->xAxis->pixelToCoord(event->pos().x());
       double y = object->parentPlot()->yAxis->pixelToCoord(event->pos().y());

        QCPItemStraightLine *line = new QCPItemStraightLine(object->parentPlot());
        line->point1->setCoords(x,y);
        line->point1->setCoords(x,y+1);
        object->parentPlot()->addItem(line);
        qDebug() << x << " - " << y;
}

void HistPlugin::mouseClick(QMouseEvent *event)
{
    if (customPlot->itemCount()!=1) {
        while (customPlot->itemCount()) {
            //qDebug() << customPlot->item(i) <<  " " << customPlot->itemCount();
            customPlot->removeItem(0);
        }
        double x = customPlot->xAxis->pixelToCoord(event->pos().x());
        double y = customPlot->yAxis->pixelToCoord(event->pos().y());
        QCPItemStraightLine *line = new QCPItemStraightLine(customPlot);
        line->point1->setCoords(x,y);
        line->point2->setCoords(x,y+1);
        customPlot->addItem(line);
    }
    else /*if (customPlot->itemCount()==1)*/ {
        double x = customPlot->xAxis->pixelToCoord(event->pos().x());
        double y = customPlot->yAxis->pixelToCoord(event->pos().y());
        QCPItemRect  *rect= new QCPItemRect(customPlot);
        rect->topLeft->setCoords(((QCPItemStraightLine*)customPlot->item(0))->point1->key(),10e8);
        rect->bottomRight->setCoords(x,10e-8);
        QBrush brush(QColor(127,127,127,127));
        rect->setBrush(brush);
        customPlot->addItem(rect);
        ModifyCube(((QCPItemStraightLine*)customPlot->item(0))->point1->key(),x);
        /*QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr(""),
                                            MESSAGE,
                                            QMessageBox::Yes | QMessageBox::No);*/
/*        if (reply == QMessageBox::Yes)
                questionLabel->setText(tr("Yes"));
        else if (reply == QMessageBox::No)
                questionLabel->setText(tr("No"));
        else
                questionLabel->setText(tr("Cancel"));*/

    }



}





void HistPlugin::Execute(HyperCube *cube, IAttributes *attr)
{


    this->cube = cube;

    customPlot->setGeometry(200,200,500,500);

    QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);

    customPlot->addPlottable(bars);
    customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);

    qint16 minValue = SHRT_MAX;
    qint16 maxValue = SHRT_MIN;
    qint16 *data = new qint16[cube->GetSizeChannel()];
    for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
        cube->GetDataChannel(i,data);
        for (unsigned int j = 0; j <cube->GetSizeChannel(); j++ ) {
            if (data[i]==-50) continue; // !!!УБРАТЬ в конечной версии!!!
            if (maxValue <= data[j]) maxValue = data[j];
            if (minValue >= data[j]) minValue = data[j];
        }
    }



    int step = (maxValue-minValue)/100;
    QVector<double> key,value;
    for (int k =0; k<100; k++) {
        key.append(maxValue+k*step);
        value.append(0);
    }


    for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
            cube->GetDataChannel(i,data);
            for (unsigned int j = 0; j <cube->GetSizeChannel(); j++ ) {
                if (data[i]==-50) continue; // !!!УБРАТЬ в конечной версии!!!
                value[100*data[j]/(maxValue-minValue)]++;
           }
    }



    bars->setData(key,value);
    customPlot->rescaleAxes();
    customPlot->setInteractions( QCP::iSelectPlottables);
    customPlot->replot();

    connect(customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),SLOT(plotLine(QCPAbstractPlottable*,QMouseEvent*)));
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)),SLOT(mouseClick(QMouseEvent*)));





    delete[] data;



    /*line = new SelectedLine(customPlot);
    line->setSelectable(true);
    line->setObjectName("Line");
    customPlot->addItem(line);
    line->setLayer("axes");*/



    //line->setSelected(true);



    customPlot->show();
}

void HistPlugin::ModifyCube(int start, int end)
{
    qint16 *data = (qint16*)cube->GetDataCube();
    for (int i = 0; i < cube->GetSizeCube(); i++) {
        if (data[i]< start) data[i] = start;
        if (data[i]>end) data[i] = end;
    }
    emit this->replot();

}







