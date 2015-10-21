#include "histplugin.h"


#include <QDebug>
#include <QProgressDialog>


#include "../Library/Utils/typeconvertor.h"


HistPlugin::HistPlugin(int HistCount, QObject *parent) : QObject(parent), HIST_COUNT(HistCount)
{
    customPlot =  new QCustomPlot();
}

HistPlugin::~HistPlugin()
{
    delete customPlot;
}


void HistPlugin::plotLine(QCPAbstractPlottable *object, QMouseEvent *event)
{
       double x = object->parentPlot()->xAxis->pixelToCoord(event->pos().x());
       double y = object->parentPlot()->yAxis->pixelToCoord(event->pos().y());

        QCPItemStraightLine *line = new QCPItemStraightLine(object->parentPlot());
        line->point1->setCoords(x,y);
        line->point1->setCoords(x,y+1);
        object->parentPlot()->addItem(line);

}

void HistPlugin::mouseClick(QMouseEvent *event)
{
    if (customPlot->itemCount()!=1) {
        while (customPlot->itemCount()) {            
            customPlot->removeItem(0);
        }
        double x = customPlot->xAxis->pixelToCoord(event->pos().x());
        double y = customPlot->yAxis->pixelToCoord(event->pos().y());
        QCPItemStraightLine *line = new QCPItemStraightLine(customPlot);
        line->point1->setCoords(x,y);
        line->point2->setCoords(x,y+1);
        customPlot->addItem(line);
    }
    else {
        double x = customPlot->xAxis->pixelToCoord(event->pos().x());
        double y = customPlot->yAxis->pixelToCoord(event->pos().y());
        QCPItemRect  *rect= new QCPItemRect(customPlot);
        rect->topLeft->setCoords(((QCPItemStraightLine*)customPlot->item(0))->point1->key(),10e8);
        rect->bottomRight->setCoords(x,10e-8);
        QBrush brush(QColor(127,127,127,127));
        rect->setBrush(brush);
        customPlot->addItem(rect);
        ModifyCube(((QCPItemStraightLine*)customPlot->item(0))->point1->key(),x);
    }

}





void HistPlugin::Execute(HyperCube *cube, IAttributes *attr)
{


    this->cube = cube;
    this->attr = (ChannelPluginAttributes *)attr;

    customPlot->setGeometry(200,200,500,500);

    QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);

    customPlot->addPlottable(bars);
    customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);

    int CHANNEL = this->attr->GetChannel();




    qint64 minValue; minValue=LONG_LONG_MAX;
    qint64 maxValue; maxValue=LONG_LONG_MIN;
    qint64 Value;

    QTime timer;
    timer.start () ;



    if (cube->GetSizeChannel()-1  < CHANNEL) CHANNEL = cube->GetSizeChannel()-1;


    char *data = new char[cube->GetSizeChannel()];

    quint64 ProgressMax = 0;
    if (CHANNEL<0) ProgressMax = cube->GetSizeCube()*2;
    else ProgressMax = cube->GetSizeChannel()*2;

    QProgressDialog progress("Формирование гистограммы", "Отменить", 0, ProgressMax);
    progress.setWindowModality(Qt::WindowModal);


    if (CHANNEL<0) {
        for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
            cube->GetDataChannel(i,data);
            for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
                LongFromCharArray(data+j*cube->GetBytesInElements(),cube->GetBytesInElements(),Value);
                if (maxValue <= Value) maxValue = Value;
                if (minValue >= Value) minValue = Value;
                if (timer.elapsed() > 3000) {
                    progress.setValue(i*cube->GetSizeChannel() + j*cube->GetBytesInElements());
                    if (progress.wasCanceled()) {
                        progress.setValue(ProgressMax);
                        delete[] data;
                        return;
                    }
                    timer.restart();
                    QApplication::processEvents();

                }
            }       
        }
    } else {
        cube->GetDataChannel(CHANNEL,data);
        for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
            LongFromCharArray(data+j*cube->GetBytesInElements(),cube->GetBytesInElements(),Value);
            if (maxValue <= Value) maxValue = Value;
            if (minValue >= Value) minValue = Value;
            if (timer.elapsed() > 3000) {
                progress.setValue(j*cube->GetBytesInElements());
                if (progress.wasCanceled()) {
                    progress.setValue(ProgressMax);
                    delete[] data;
                    return;
                }
                timer.restart();
                QApplication::processEvents();

            }
        }


    }

    progress.setValue(ProgressMax/2);


    int step = (maxValue-minValue)/HIST_COUNT;

    QVector<double> key,value;

    for (int k =0; k<HIST_COUNT+1; k++) {
        key.append(minValue+k*step);
        value.append(0);
    }


    if (CHANNEL<0) {
        for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
                cube->GetDataChannel(i,data);
                for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
                    LongFromCharArray(data+j*cube->GetBytesInElements(),cube->GetBytesInElements(),Value);
                    value[(Value-minValue)/((maxValue-minValue)/HIST_COUNT)]++;
                    if (timer.elapsed() > 3000) {
                        progress.setValue(ProgressMax+i*cube->GetSizeChannel() + j*cube->GetBytesInElements());
                        if (progress.wasCanceled()) {
                            progress.setValue(ProgressMax);
                            delete[] data;
                            return;
                        }
                        timer.restart();
                        QApplication::processEvents();

                    }
                }

        }
    } else {
        cube->GetDataChannel(CHANNEL,data);
        for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
            LongFromCharArray(data+j*cube->GetBytesInElements(),cube->GetBytesInElements(),Value);
            value[(Value-minValue)/((double)(maxValue-minValue)/HIST_COUNT)]++;
            if (timer.elapsed() > 3000) {
                progress.setValue(ProgressMax+j*cube->GetBytesInElements());
                if (progress.wasCanceled()) {
                    progress.setValue(ProgressMax);
                    delete[] data;
                    return;
                }
                timer.restart();
                QApplication::processEvents();
            }
        }
    }

    progress.setValue(ProgressMax);

    bars->setData(key,value);
    customPlot->rescaleAxes();
    customPlot->setInteractions( QCP::iSelectPlottables);
    customPlot->replot();

    connect(customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),SLOT(plotLine(QCPAbstractPlottable*,QMouseEvent*)));
    connect(customPlot, SIGNAL(mousePress(QMouseEvent*)),SLOT(mouseClick(QMouseEvent*)));

    delete[] data;

    customPlot->show();
}

void HistPlugin::ModifyCube(int start, int end)
{

    int CHANNEL = this->attr->GetChannel();



    char *OneElement = new char[cube->GetBytesInElements()];
    char *MinElement = new char[cube->GetBytesInElements()];
    char *MaxElement = new char[cube->GetBytesInElements()];
    memset(MinElement,start,cube->GetBytesInElements());
    memset(MaxElement,end,cube->GetBytesInElements());



    QTime timer;
    timer.start () ;

    if (CHANNEL<0) {
        QProgressDialog progress("Подождите, пожалуйста", "Отменить", 0, cube->GetSizeCube());
        progress.setWindowModality(Qt::WindowModal);
        char **data = (char**)cube->GetDataCube();
        for (int i = 0; i < cube->GetCountofChannels(); i++) {
            for (int j = 0; j < cube->GetSizeChannel()/cube->GetBytesInElements(); j++){
                memset(OneElement,0,cube->GetBytesInElements());
                memcpy(OneElement, data[i] + j*cube->GetBytesInElements(),cube->GetBytesInElements());
                if (memcmp(OneElement,MinElement,cube->GetBytesInElements())<0) memcpy(data[i] + j*cube->GetBytesInElements(),MinElement,cube->GetBytesInElements());
                if (memcmp(OneElement,MaxElement,cube->GetBytesInElements())>0) memcpy(data[i] + j*cube->GetBytesInElements(),MaxElement,cube->GetBytesInElements());
                if (timer.elapsed() > 1000) {
                    progress.setValue(i*cube->GetSizeChannel() + j*cube->GetBytesInElements());
                    if (progress.wasCanceled()) {
                        progress.setValue(cube->GetSizeCube());
                        delete[] OneElement;
                        delete[] MinElement;
                        delete[] MaxElement;
                        return;
                    }
                    timer.restart();
                    QApplication::processEvents();
                }
            }
        }
    progress.setValue(cube->GetSizeCube());


    } else {
            QProgressDialog progress("Подождите, пожалуйста", "Отменить", 0, cube->GetSizeChannel());
            progress.setWindowModality(Qt::WindowModal);
            char *data = new char[cube->GetSizeChannel()];
            cube->GetDataChannel(CHANNEL,data);
            for (int j = 0; j < cube->GetSizeChannel()/cube->GetBytesInElements(); j++){
                memset(OneElement,0,cube->GetBytesInElements());
                memcpy(OneElement, data + j*cube->GetBytesInElements(),cube->GetBytesInElements());
                if (memcmp(OneElement,MinElement,cube->GetBytesInElements())<0) memcpy(data + j*cube->GetBytesInElements(),MinElement,cube->GetBytesInElements());
                if (memcmp(OneElement,MaxElement,cube->GetBytesInElements())>0) memcpy(data + j*cube->GetBytesInElements(),MaxElement,cube->GetBytesInElements());
                if (timer.elapsed() > 1000) {
                    progress.setValue(j*cube->GetBytesInElements());
                    if (progress.wasCanceled()) {
                        progress.setValue(cube->GetSizeCube());
                        delete[] OneElement;
                        delete[] MinElement;
                        delete[] MaxElement;
                        delete[] data;
                        return;
                    }
                    timer.restart();
                    QApplication::processEvents();
                }
            }
    progress.setValue(cube->GetSizeChannel());
    delete[] data;
    }


    delete[] OneElement;
    delete[] MinElement;
    delete[] MaxElement;

    //emit this->replot();

}







