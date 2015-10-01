#include "PlotterWindow.h"
#include "ui_PlotterWindow.h"
#include <QtAlgorithms>
#include <../Library/GenericExc.h>

PlotterWindow::PlotterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlotterWindow)
    , minY(70000)
    , maxY(-10000)

{
    setWindowIcon(QIcon(":/logo/IconsPlotter/PlotterLogo.ico"));
    ui->setupUi(this);
    m_customPlot = (QCustomPlot*) ui->PlotWidget;
    m_hold = false;
    initSize = size();
    connect(m_customPlot,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(wheelEventCustomPlotActivated()));
    qDebug()<<"init size"<<initSize;
    QPropertyAnimation* panim = new QPropertyAnimation(this, "windowOpacity");

    panim->setDuration(300);
    panim->setStartValue(0);
    panim->setEndValue(1);
    panim->setEasingCurve(QEasingCurve::InCirc);
    panim->start();
}

PlotterWindow::~PlotterWindow()
{
    delete ui;
}

void PlotterWindow::plotSpectr(HyperCube *ptrCube, uint dataX, uint dataY)
{

    quint16 Chnls = ptrCube->GetCountofChannels();
    qint16* pSpectrValues = new qint16[Chnls];
    try{    //если можем получить точку гиперкуба
        ptrCube->GetSpectrumPoint(dataX, dataY,pSpectrValues); // записали в pSpectrValues из гиперкуба
        QList<double> Wawes;
        Wawes = ptrCube->GetListOfChannels();
        QVector<double> xArr(Chnls), yArr(Chnls);
        for (quint16 i = 0; i < Chnls; ++i )
        {
           xArr[i] = Wawes[i];
           yArr[i] = pSpectrValues[i];
        }

        QVector<double> sortedYArr;
        sortedYArr = yArr;
        qSort(sortedYArr);
        if (sortedYArr.first() < minY )
            minY = sortedYArr.first();
        if (sortedYArr.last() > maxY )
            maxY = sortedYArr.last();

        QString grafName;
        grafName.append("X:");
        grafName.append(QString::number(dataX));
        grafName.append(" Y:");
        grafName.append(QString::number(dataY));
        m_customPlot->setInteraction(QCP::iRangeDrag , true);
        m_customPlot->setInteraction(QCP::iRangeZoom  , true);

        m_customPlot->legend->setVisible(true);
        if (!m_hold)
            m_customPlot->clearGraphs();

        m_customPlot->addGraph();
        if (m_customPlot->graphCount() == 1) // первый график всегда черного цвета, остальные - рандомные
            m_customPlot->graph()->setPen(QPen(Qt::black));
        else
        {
           QColor color;
           int limit = 256;
           int randR = qrand() % limit;
           int randG = qrand() % limit;
           int randB = qrand() % limit;
           color.setRgb(randR,randG,randB);
           m_customPlot->graph()->setPen(QPen(color));
        }
        m_customPlot->graph()->setName(grafName);
        m_customPlot->graph()->setData(xArr,yArr);
        m_customPlot->xAxis->setRange(xArr.first(),xArr.last());
        m_customPlot->yAxis->setRange(minY,maxY);
        m_customPlot->xAxis->setLabel("Длина волны, нм");
        m_customPlot->yAxis->setLabel("Яркость");

        m_customPlot->replot();
        autoXStep = m_customPlot->xAxis->tickStep();
        newXStep = autoXStep;

    } catch(const GenericExc& exc)
    {
        m_customPlot->replot();
    } catch(...)
    {
        m_customPlot->replot();
    }



}

void PlotterWindow::chooseStep()
{


    if(newXStep < autoXStep)
    {
//        m_customPlot->xAxis->setAutoTickStep(false);
        m_customPlot->xAxis->setTickStep( newXStep);

    }
    else
        m_customPlot->xAxis->setTickStep( autoXStep);
}

void PlotterWindow::resizeEvent(QResizeEvent *event)
{

    qDebug()<<"step"<<autoXStep;
    width = event->size().width();
    double tempNewXStep = autoXStep / width * initSize.width();
    if(tempNewXStep > 10)
        newXStep = round(autoXStep / width * initSize.width());
    else
        newXStep = tempNewXStep;
    m_customPlot->xAxis->setAutoTickStep(false);
    chooseStep();
    qDebug()<<"width"<<width<<" iniewidth"<<initSize.width();
}

void PlotterWindow::wheelEvent(QWheelEvent *)
{

}

void PlotterWindow::on_actionHold_toggled(bool value)
{
    m_hold = value;
}

void PlotterWindow::wheelEventCustomPlotActivated()
{
     m_customPlot->xAxis->setAutoTickStep(true);
     autoXStep = m_customPlot->xAxis->tickStep();
//     chooseStep();
     resize(this->size());
}
