#include "PlotterAlongLine.h"
#include "qmath.h"


PlotterAlongLine::PlotterAlongLine(QWidget *parent) : QMainWindow(parent)
{
    setupUI();


}
PlotterAlongLine::~PlotterAlongLine()
{

}
void PlotterAlongLine::setupUI()
{
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    pPlotWidget = new QCustomPlot(centralWidget);
    pPlotWidget->setObjectName(QStringLiteral("PlotWidget"));
    verticalLayout->addWidget(pPlotWidget);
    this->setCentralWidget(centralWidget);
    this->setWindowTitle("Яркость вдоль прямой");
    this->resize(518,360);
    QPalette palette;
    QBrush brush1(QColor(255, 255, 255, 255));
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    this->setPalette(palette);
}


void PlotterAlongLine::plotSpctr(HyperCube* pCube, uint x1, uint x2, uint y1, uint y2, uint z1, uint z2) // это data координаты
{
    //    QVector<double> xArr(Chnls), yArr(Chnls);
    //    qint16* pSpectrValues = new qint16[Chnls];
    //    ptrCube->GetSpectrumPoint(dataX, dataY,pSpectrValues);
    uint k = 3;
    int length = round (qSqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1)) / k);

    uint* cordXarr = new uint[length];
    uint* cordYarr = new uint[length];
    uint* cordZarr = new uint[length];
    for (int i = 0; i<length; ++i)
    {
        cordXarr[i] = (uint)(x1 + (double)(x2-x1)/(length-1) * i);
        cordYarr[i] = (uint)(y1 + (double)(y2-y1)/(length-1) * i);
        cordZarr[i] = (uint)(z1 +(double)(z2-z1)/(length-1) * i);

    }
    QVector<double> plotXArr(length), plotYArr(length);

    quint16 Chnls = pCube->GetCountofChannels();
    for (uint i = 0; i<length; ++i)
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

    pPlotWidget->setInteraction(QCP::iRangeDrag , true);
    pPlotWidget->setInteraction(QCP::iRangeZoom  , true);
   // pPlotWidget->legend->setVisible(true);
    pPlotWidget->addGraph();
    pPlotWidget->graph()->setPen(QPen(Qt::black));

    //pPlotWidget->graph()->setName("grafName");
    pPlotWidget->graph()->setData(plotXArr,plotYArr);
    pPlotWidget->xAxis->setRange(plotXArr.first(),plotXArr.last());
    pPlotWidget->yAxis->setRange(minY,maxY);
    pPlotWidget->replot();





}

