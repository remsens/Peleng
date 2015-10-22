#include "Main2DWindow.h"
#include "ui_Main2DWindow.h"
#include "../Library/PluginAttributes/ChannelPluginAttributes.h"
#include "../Library/PluginAttributes/Cube3DPluginAttributes.h"
#include "../Library/PelengPluginLoader.h"
#include "hist/histplugin.h"

int cmp2(const void *a, const void *b);

Main2DWindow::Main2DWindow(HyperCube *pHyperCube,int chan,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main2DWindow),
    m_initChanel(chan),
    m_dataX(0), m_dataY(0),
    firstWindowPlotter(true)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/IconsCube/iconsCube/Heat Map-50.png"));//!!! почему-то можно поставить иконку только из qrc файла проекта 3Dcube (наверное, надо заинклудить qrc 2d куба в 3d проект)
    QPropertyAnimation* panim = new QPropertyAnimation(this, "windowOpacity");
    panim->setDuration(300);
    panim->setStartValue(0);
    panim->setEndValue(1);
    panim->setEasingCurve(QEasingCurve::InCirc);
    panim->start(QAbstractAnimation::DeleteWhenStopped);

    setInitCustomplotSettings();
    createMenus();
    setHyperCube(pHyperCube);
    fillChanList();
    connect(ui->customPlot,SIGNAL(customContextMenuRequested(QPoint)),SLOT(contextMenuRequest(QPoint)));
    //connect(ui->customPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(prepareToPlotSpectr())); //сделать как в GLwidget

    //connect(ui->customPlot,SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),SLOT(colorMapClicked(QCPAbstractPlottable*,QMouseEvent*)));
    //connect(ui->customPlot,SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseMoveOnColorMap(QMouseEvent*)));
    connect(ui->customPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(mousePressOnColorMap(QMouseEvent*)));

    ui->listWidget->setCurrentRow(m_initChanel);
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(updateViewchan(int))); //раскомментить
    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));
    drawHeatMap(m_initChanel);

}

Main2DWindow::~Main2DWindow()
{
    delete ui;
}

void Main2DWindow::setInitChanel(u::uint32 initChanel)
{
    m_initChanel = initChanel;
    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));
    drawHeatMap(m_initChanel);
}

void Main2DWindow::setHyperCube(HyperCube *ptrCube)
{
    m_pCube = ptrCube;
    rows = m_pCube->GetLines();
    cols = m_pCube->GetColumns();
    data = (qint16**)ptrCube->GetDataCube();
    colorMap->data()->setSize(rows, cols);
    colorMap->data()->setRange(QCPRange(0, rows-1), QCPRange(0, cols-1));
    QSize mainSize = this->size();
    this->resize(mainSize.width()*1.5, mainSize.width() * cols / rows*1.5);
//    ui->customPlot->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//    ui->customPlot->resize(CPlotSize.width() ,CPlotSize.width() * cols / rows);
}

void Main2DWindow::setInitCustomplotSettings()
{
    colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->yAxis->setTicks(false);
    ui->customPlot->xAxis->setTicks(false);
    ui->customPlot->xAxis->setTickLabels(false);
    ui->customPlot->yAxis->setTickLabels(false);
    ui->customPlot->xAxis->setVisible(false);
    ui->customPlot->yAxis->setVisible(false);
    ui->customPlot->axisRect()->setAutoMargins(QCP::msNone);
    ui->customPlot->axisRect()->setMargins(QMargins(0,0,0,-1));// -1 устраняет баг с полосой белых пикселей при 0
    colorMap->setKeyAxis(ui->customPlot->xAxis);
    colorMap->setValueAxis(ui->customPlot->yAxis);
    ui->customPlot->addPlottable(colorMap);
    ui->customPlot->setInteraction(QCP::iRangeDrag , true);
    ui->customPlot->setInteraction(QCP::iRangeZoom  , true);
}
void Main2DWindow::fillChanList()
{
    QList<double> list = m_pCube->GetListOfChannels();
    int num = 0;
    foreach(double i , list) {
        ui->listWidget->addItem(QString("%1 - %2 нм").arg(num).arg(i));
        num++;
    }
}

void Main2DWindow::drawHeatMap(int chan)
{
    qint16 *dat =  new qint16[rows * cols];
    m_pCube->GetDataChannel(chan,dat);
    for (int x=0; x < rows; ++x) {
        for (int y=0; y < cols; ++y) {
            colorMap->data()->setCell(x, y,dat[x * cols + y] );
        }
    }
    ui->customPlot->rescaleAxes();
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->rescaleDataRange(true);
    int minCMap, maxCMap;
    findMinMaxforColorMap(chan,minCMap, maxCMap);
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    qDebug()<<"chan: "<<chan;
    delete dat;
}
void Main2DWindow::findMinMaxforColorMap(int chan, int &minCMap, int &maxCMap,float thresholdLow,float thresholdHigh)
//thresholdLow = 0.02 (первые 2% игнорируются), thresholdHigh = 0.98
{
    minCMap =  32767;
    maxCMap = -32767;
    qint16 *dataTemp = new qint16[rows*cols];
    for (int j = 0; j<rows*cols; ++j)
        dataTemp[j]=data[chan][j];
    QElapsedTimer timer3;
    timer3.start();
    qsort(dataTemp,cols*rows,sizeof(qint16),cmp2);
    qDebug()<<"сортировка"<<timer3.elapsed()<< " мс";
    minCMap = dataTemp[int(rows*cols*thresholdLow)];
    maxCMap = dataTemp[int(rows*cols*thresholdHigh)];
    delete[] dataTemp;
}

int cmp2(const void *a, const void *b)
{
    const qint16 *pa = (const qint16*)a;
    const qint16 *pb = (const qint16*)b;
    if (*pa < *pb)
        return -1;
    else if (*pa > *pb)
        return +1;
    else
        return 0;
}

void Main2DWindow::updateViewchan(int chan)
{
    drawHeatMap( chan);
    qDebug() << "Replot heatmap";
}


void Main2DWindow::mousePressOnColorMap( QMouseEvent *e)
{
    int x = this->ui->customPlot->xAxis->pixelToCoord(e->pos().x());
    int y = this->ui->customPlot->yAxis->pixelToCoord(e->pos().y());
    if (x >= 0 && x < rows)
        m_dataX = x;
    if (y >= 0 && y < cols)
        m_dataY = y;
    int chan = ui->listWidget->currentRow();
    qDebug()<<"X:"<<m_dataX<<" Y:"<<m_dataY <<" яркость:"<<data[chan][m_dataX * cols + m_dataY];
}

void Main2DWindow::createMenus()
{
    pContextMenu = new QMenu();
    pContextMenu->setStyleSheet("border: 0px solid black;");
    pPlotAction = new QAction(QIcon(":/IconsCube/iconsCube/Plot.ico"),"Спектр",this);
    pDeletePlotsAction = new QAction(QIcon(":/IconsCube/iconsCube/close.ico"),"Закрыть окна спектров",this);
    pPlotLineAction = new QAction("Спектральный срез", this);
    pPlotHistAction = new QAction("Гистограмма",this);
    pContextMenu->addAction(pPlotAction);
    //pContextMenu->addAction(pDeletePlotsAction);
   // pContextMenu->addAction(pPlotLineAction);
    pContextMenu->addAction(pPlotHistAction);
    connect(pPlotAction,SIGNAL(triggered()),SLOT(prepareToPlotSpectr()));
    connect(pPlotHistAction,SIGNAL(triggered()),SLOT(prepareToHist()));
//    connect(pDeletePlotsAction,SIGNAL(triggered()),SLOT(deleteSpectrWindows()));
//    connect(this,SIGNAL(sendXYZ(uint,uint,uint)),SLOT(plotSpectr(uint,uint,uint) ));
    connect(this, SIGNAL(signalPlotAlongLine(uint,uint,uint,uint,uint,uint)),SLOT(plotAlongLine(uint,uint,uint,uint,uint,uint)));
    connect(pPlotLineAction,SIGNAL(triggered()),SLOT(createLinePlotterSlot()));


}
void Main2DWindow::plotSpectr(uint x, uint y)
{
    if (firstWindowPlotter || windowPlotter->getIsHold() == false)// если не стоит чекбокс Hold, то создается новый объект,
    {                                                             // иначе - графики строятся в том же окне (объекте)
        windowPlotter = new PlotterWindow();
        QObject::connect(windowPlotter, SIGNAL(closePlotterWindow(PlotterWindow*)), this, SLOT(DeleteSpectrWindow(PlotterWindow*)));
        windowsArr.append(windowPlotter);
        firstWindowPlotter = false;
    }

    windowPlotter->plotSpectr(m_pCube,x,y);
    windowPlotter->activateWindow();
    windowPlotter->show();

}

void Main2DWindow::prepareToPlotSpectr()
{
    plotSpectr(m_dataX,m_dataY);
}
void Main2DWindow::createLinePlotterSlot()
{
    linePlotterIsActive = true;
    QString strForLineHelp = "Выберите начальную точку";
    setCursor(QCursor(QPixmap(":/IconsLine/iconsLine/start_flag.png"),10,29));
    //emit flagsToolTip(globalPos,"выберите начальную точку");
    connect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(startIsClicked()));
    pContextMenu->hide();
    this->setToolTip(strForLineHelp);
}
void Main2DWindow::startIsClicked()
{
    linePlotterIsActive = true;
    m_x1 = m_dataX;
    m_y1 = m_dataY;
    m_z1 = ui->listWidget->currentRow();
    //emit flagsToolTip(globalPos,"выберите конечную точку");
    QString strForLineHelp = "выберите конечную точку";
    this->setToolTip(strForLineHelp);
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/finish_flag.png"),10,29));
    disconnect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(startIsClicked()));
    connect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(finishIsClicked()));
}
void Main2DWindow::finishIsClicked()
{
    linePlotterIsActive = false;
    m_x2 = m_dataX;
    m_y2 = m_dataY;
    m_z2 = ui->listWidget->currentRow();
    QString strForLineHelp = "";
    setCursor(Qt::ArrowCursor);
    //emit signalPlotAlongLine(m_x1, m_x2, m_y1, m_y2, m_z1, m_z2);
    disconnect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(startIsClicked()));
    disconnect(this,SIGNAL(signalCurrentDataXYZ(uint,uint,uint)),this,SLOT(finishIsClicked()));
    this->setToolTip(strForLineHelp);
    //emit flagsToolTip(globalPos,"");
}
void Main2DWindow::plotAlongLine(uint x1, uint x2, uint y1, uint y2, uint z1, uint z2)
{
    pWidgLine = new LinePlotterWindow();
    QObject::connect(pWidgLine, SIGNAL(closeLinePlotterWindow(LinePlotterWindow*)), this, SLOT(DeleteLineWindow(LinePlotterWindow*)));
    windowsLineArr.append(pWidgLine);
    pWidgLine->plotSpectrLine(m_pCube,x1,x2,y1,y2,z1,z2);
    pWidgLine->activateWindow();
    pWidgLine->show();
}

void Main2DWindow::prepareToHist()
{
   /* PelengPluginLoader pelengLoade
    * r;
    PelengPluginsInterface* m_pelengPlugins = pelengLoader.LoadPlugin("Hist UI");
    IAttributes* attrCh = new ChannelPluginAttributes(3);
     m_pelengPlugins->Execute(m_pCube, attrCh);*/

    //QList<QString> m;
    //IAttributes* attrCh = new Cube3DPluginAttributes();
    u::uint32 channel = ui->listWidget->currentRow();
    IAttributes* attrCh = new ChannelPluginAttributes(channel);
    HistPlugin *hist = new HistPlugin(100, this);
    hist->Execute(m_pCube,attrCh);
    connect(hist,SIGNAL(replot()),SLOT(updateViewchan()));
}

void Main2DWindow::contextMenuRequest(QPoint point)
{

    pContextMenu->exec(ui->customPlot->mapToGlobal(point));//centralwidget->viewport()->mapToGlobal(point));

}







