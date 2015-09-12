#include "PlotterWindow.h"
#include "ui_PlotterWindow.h"
#include <QtAlgorithms>
PlotterWindow::PlotterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotterWindow)
{
    ui->setupUi(this);
    m_customPlot = (QCustomPlot*) ui->PlotWidget;
    m_hold = false;

    QPropertyAnimation* panim2 = new QPropertyAnimation(this, "windowOpacity");

    panim2->setDuration(300);
    panim2->setStartValue(0);
    panim2->setEndValue(1);
    panim2->setEasingCurve(QEasingCurve::InCirc);
    panim2->start();
}

PlotterWindow::~PlotterWindow()
{
    delete ui;
}

void PlotterWindow::plotSpectr(HyperCube *ptrCube, uint dataX, uint dataY)
{

    quint16 Chnls = ptrCube->GetCountofChannels();
    qint16* pSpectrValues = new qint16[Chnls];
    try{
        ptrCube->GetSpectrumPoint(dataX, dataY,pSpectrValues); // записали в pSpectrValues из гиперкуб
        QVector<double> xArr(Chnls), yArr(Chnls);
        for (quint16 i = 0; i < Chnls; ++i )
        {
           xArr[i] = i;
           yArr[i] = pSpectrValues[i];
        }
        QVector<double> sortedYArr;
        sortedYArr = yArr;
        qSort(sortedYArr);
        if (!m_hold)
            m_customPlot->clearGraphs();
        m_customPlot->addGraph();
        m_customPlot->graph()->setData(xArr,yArr);
        m_customPlot->xAxis->setRange(xArr.first(),xArr.last());
        m_customPlot->yAxis->setRange(sortedYArr.first(),sortedYArr.last());
        m_customPlot->graph()->setPen(QPen(Qt::black));
        m_customPlot->replot();
    }catch(...){
        m_customPlot->clearGraphs();
        m_customPlot->replot();
    }



}

void PlotterWindow::on_actionHold_toggled(bool value)
{
    m_hold = value;
}
