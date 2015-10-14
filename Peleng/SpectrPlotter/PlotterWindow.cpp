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
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowIcon(QIcon(":/logo/IconsPlotter/PlotterLogo.ico"));
    ui->setupUi(this);
    m_customPlot = (QCustomPlot*) ui->PlotWidget;
    m_hold = false;
    initSize = size();

    qDebug()<<"init size"<<initSize;
    QPropertyAnimation* panim = new QPropertyAnimation(this, "windowOpacity");
    panim->setDuration(300);
    panim->setStartValue(0);
    panim->setEndValue(1);
    panim->setEasingCurve(QEasingCurve::InCirc);
    panim->start(QAbstractAnimation::DeleteWhenStopped);
}

PlotterWindow::~PlotterWindow()
{
    delete ui;
}

void PlotterWindow::closeEvent(QCloseEvent *) {
    emit closePlotterWindow(this);
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
        autoTickCountX = m_customPlot->xAxis->autoTickCount();
        autoTickCountY = m_customPlot->yAxis->autoTickCount();
    } catch(const GenericExc& exc)
    {
        m_customPlot->replot();
    } catch(...)
    {
        m_customPlot->replot();
    }
}

void PlotterWindow::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width();
    int height = event->size().height();
    int TickCountX = round(autoTickCountX * (double)width / initSize.width());
    int TickCountY = round(autoTickCountY * (double)height / initSize.height());
    m_customPlot->xAxis->setAutoTickCount(TickCountX);
    m_customPlot->yAxis->setAutoTickCount(TickCountY);
    m_customPlot->replot();
}

void PlotterWindow::on_actionHold_toggled(bool value)
{
    m_hold = value;
}

