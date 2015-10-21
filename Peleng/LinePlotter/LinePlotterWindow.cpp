#include "LinePlotterWindow.h"
#include "qmath.h"



LinePlotterWindow::LinePlotterWindow(QWidget *parent) : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setupUI();
    setWindowIcon(QIcon(":/IconsLine/iconsLine/PlotterLogo.ico"));
    QPropertyAnimation* panim = new QPropertyAnimation(this, "windowOpacity");
    panim->setDuration(300);
    panim->setStartValue(0);
    panim->setEndValue(1);
    panim->setEasingCurve(QEasingCurve::InCirc);
    panim->start();

}

LinePlotterWindow::~LinePlotterWindow()
{
    delete centralWidget;
}

void LinePlotterWindow::closeEvent(QCloseEvent *) {
    emit closeLinePlotterWindow(this);
}

void LinePlotterWindow::plotSpectrLine(HyperCube *pCube, uint x1, uint x2, uint y1, uint y2, uint z1, uint z2)
{
    m_customPlot->clearGraphs(); // только 1 график на виджете
    uint k = 1;
    int length = round (qSqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1)) / k);

    int* cordXarr = new int[length];
    int* cordYarr = new int[length];
    int* cordZarr = new int[length];
    for (int i = 0; i<length; ++i)
    {
        cordXarr[i] = x1 + (double)((int)x2-(int)x1)/(length-1) * i;
        cordYarr[i] = y1 + (double)((int)y2-(int)y1)/(length-1) * i;
        cordZarr[i] = z1 +(double)((int)z2-(int)z1)/(length-1) * i;

    }
    QVector<double> plotXArr(length), plotYArr(length);

    quint16 Chnls = pCube->GetCountofChannels();
    for (int i = 0; i<length; ++i)
    {
        qint16* pSpectrValues = new qint16[Chnls];
        try{
             pCube->GetSpectrumPoint(cordXarr[i], cordYarr[i],pSpectrValues);
        }catch(...)
        {
            qDebug()<<"не берет GetSpectrumPoint";
        }

        plotYArr[i] = pSpectrValues[cordZarr[i]]; // эти три строчки можно заменить GetSpectrum1Point, если реализовать ее в гиперкубе
        plotXArr[i] = i;
        delete pSpectrValues;
    }
    delete cordXarr;
    delete cordYarr;
    delete cordZarr;

    QVector<double> sortedYArr;
    sortedYArr = plotYArr;
    qSort(sortedYArr);
     double minY = 70000, maxY = -10000;
    if (sortedYArr.first() < minY )
        minY = sortedYArr.first();
    if (sortedYArr.last() > maxY )
        maxY = sortedYArr.last();

    m_customPlot->setInteraction(QCP::iRangeDrag , true);
    m_customPlot->setInteraction(QCP::iRangeZoom  , true);
   // pPlotWidget->legend->setVisible(true);
    m_customPlot->addGraph();
    m_customPlot->graph()->setPen(QPen(Qt::black));

    //pPlotWidget->graph()->setName("grafName");
    m_customPlot->graph()->setData(plotXArr,plotYArr);
    m_customPlot->xAxis->setRange(plotXArr.first(),plotXArr.last());
    m_customPlot->yAxis->setRange(minY,maxY);
    m_customPlot->replot();
}

void LinePlotterWindow::setupUI()
{
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    m_customPlot = new QCustomPlot(centralWidget);
    m_customPlot->setObjectName(QStringLiteral("PlotWidget"));
    verticalLayout->addWidget(m_customPlot);
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("Яркость вдоль прямой");
    this->resize(518,360);
    QPalette palette;
    QBrush brush1(QColor(255, 255, 255, 255));
    palette.setBrush(QPalette::All, QPalette::Window, brush1);
    this->setPalette(palette);

}

