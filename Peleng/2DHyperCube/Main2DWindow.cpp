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
    firstWindowPlotter(true),
    linePlotterIsActive(false),
    m_initChanel(chan),
    m_dataX(0), m_dataY(0),
    m_interplolate(false),
    flagSlidersEnabledForSlots(false),
    flagPolygonIsCreated(true),
    flagDoubleClicked(false)

{
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);
    pStatusBarLabel = new QLabel(this);
    ui->statusbar->addWidget(pStatusBarLabel);
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
    initArrChanLimits();
    fillChanList();



    connect(ui->actionInterpolation,SIGNAL(toggled(bool)),SLOT(toggledActionInterpolation(bool)));
    connect(ui->customPlot,SIGNAL(customContextMenuRequested(QPoint)),SLOT(contextMenuRequest(QPoint)));

    connect(ui->customPlot,SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseMoveOnColorMap(QMouseEvent*)));
    connect(ui->customPlot,SIGNAL(mouseDoubleClick(QMouseEvent*)),SLOT(mouseDblClickOnColorMap(QMouseEvent*)));
    connect(ui->customPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(mousePressOnColorMap(QMouseEvent*)));
    ui->listWidget->setCurrentRow(m_initChanel);
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(updateViewchan(int)));
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(setInitSliders(int)));
    connect(ui->SliderContrastMin,SIGNAL(valueChanged(int)),SLOT(leftBorderContrast(int)));
    connect(ui->SliderContrastMax,SIGNAL(valueChanged(int)),SLOT(rightBorderContrast(int)));
    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));
    emit  ui->listWidget->currentRowChanged(m_initChanel);




    QSize mainSize = this->size();
    this->resize(mainSize.width()*1.5, mainSize.width() * cols / rows*1.5);

    ui->customPlot->setMinimumSize(this->size().width() * 0.75 ,this->size().width() * 0.75* cols / rows);
    ui->customPlot->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
}

Main2DWindow::~Main2DWindow()
{
    delete ui;
}

void Main2DWindow::resizeEvent(QResizeEvent *e)
{
    ui->customPlot->setMinimumWidth(1);
    ui->customPlot->setFixedHeight( ui->customPlot->width()* cols / rows);

}

void Main2DWindow::setInitChanel(u::uint32 initChanel)
{
    m_initChanel = initChanel;
    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));
    //или update
    int minCMap, maxCMap;
    findMinMaxforColorMap(m_initChanel,minCMap, maxCMap,0.04, 0.98);
    drawHeatMap(m_initChanel,minCMap, maxCMap);
}

void Main2DWindow::setHyperCube(HyperCube *ptrCube)
{
    m_pCube = ptrCube;
    rows = m_pCube->GetLines();
    cols = m_pCube->GetColumns();
    chnls = m_pCube->GetCountofChannels();
    data = (qint16**)ptrCube->GetDataCube();
    colorMap->data()->setSize(rows, cols);
    colorMap->data()->setRange(QCPRange(0, rows-1), QCPRange(0, cols-1));



//    ui->customPlot->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    ui->customPlot->setMinimumSize( this->size().width()*0.75 ,this->size().width()*0.75 * cols / rows );
    //QSize mainSize = this->size();
    //this->resize(mainSize.width()*1.5, mainSize.width() * cols / rows*1.5);
//    resizeEvent(QResizeEvent(this->size(),this->size()));
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
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->rescaleDataRange(true);

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

void Main2DWindow::initArrChanLimits()
{
    ChnlLimits = new int*[chnls];
    for (int i = 0; i < chnls; ++i)
        ChnlLimits[i] = new int[2];

    for (int i = 0; i < chnls; ++i)
    {
        ChnlLimits[i][0] = -32767;
        ChnlLimits[i][1] = -32767;
    }
}

void Main2DWindow::drawHeatMap(int chan, int minCMap, int maxCMap)
{
    QElapsedTimer timer;
    timer.start();
    qint16 *dat =  new qint16[rows * cols];
    m_pCube->GetDataChannel(chan,dat);
    for (int x=0; x < rows; ++x) {
        for (int y=0; y < cols; ++y) {
            colorMap->data()->setCell(x, y,dat[x * cols + y] );
        }
    }
    ui->customPlot->rescaleAxes();
    colorMap->setInterpolate(m_interplolate);
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    ui->customPlot->replot();
    delete dat;
    qDebug()<<"drawHeatMap"<<timer.elapsed()<< " ms";
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
    if(ChnlLimits[chan][0] == -32767 || ChnlLimits[chan][1] == -32767 ) //мб переделать, что если после findMinMaxforColorMap minCMap = 0
    {
        int minCMap, maxCMap;
        findMinMaxforColorMap(chan,minCMap, maxCMap,0.04, 0.98);
        ChnlLimits[chan][0] = minCMap;
        ChnlLimits[chan][1] = maxCMap;
    }

    drawHeatMap(chan,ChnlLimits[chan][0], ChnlLimits[chan][1]);
}

void Main2DWindow::leftBorderContrast(int left)
{
    if (flagSlidersEnabledForSlots)
    {
        int chan = ui->listWidget->currentRow();
        ChnlLimits[chan][0] = left;
        drawHeatMap(chan,ChnlLimits[chan][0], ChnlLimits[chan][1]);
        qDebug()<<"leftBorderContrast chan:"<<chan;
    }
}

void Main2DWindow::rightBorderContrast(int right)
{
    if (flagSlidersEnabledForSlots)
    {
        int chan = ui->listWidget->currentRow();
        ChnlLimits[chan][1] = right;
        drawHeatMap(chan,ChnlLimits[chan][0], ChnlLimits[chan][1]);
        qDebug()<<"rightBorderContrast chan:"<<chan;
    }
}

void Main2DWindow::contrastImage(int left, int right)//left,right -  левая и правая граница гистограммы каналла. Т.е. 2 значения яркостей в data
{
    int chan = ui->listWidget->currentRow();
    if (left > right)
        std::swap(left,right);
    ChnlLimits[chan][0] = left;
    ChnlLimits[chan][1] = right;
    updateViewchan(chan);
    flagSlidersEnabledForSlots = false;
    ui->SliderContrastMin->setValue(ChnlLimits[chan][0]);
    ui->SliderContrastMax->setValue(ChnlLimits[chan][1]);
    flagSlidersEnabledForSlots = true;
    qDebug()<<"slot contrast "<<left<<" "<<right;
}

void Main2DWindow::plotPointsOn2D(QVector<double> x, QVector<double> y)
{
    ui->customPlot->clearGraphs(); //удаляем предыдущий график
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x, y);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsNone);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::red));
    myScatter.setBrush(Qt::yellow);
    //myScatter.setSize(5);
    ui->customPlot->graph()->setScatterStyle(myScatter);
    ui->customPlot->replot();
}


void Main2DWindow::mousePressOnColorMap(QMouseEvent *e)
{
    if(flagDoubleClicked)
    {
        flagDoubleClicked  = false;
        return;
    }
    int x = this->ui->customPlot->xAxis->pixelToCoord(e->pos().x());
    int y = this->ui->customPlot->yAxis->pixelToCoord(e->pos().y());
    if(x<0)
        m_dataX = 0;
    else if(x>rows)
        m_dataX = rows-1;
    else
        m_dataX = x;

    if(y<0)
        m_dataY = 0;
    else if(y>cols)
        m_dataY = cols-1;
    else
        m_dataY = y;

    emit signalCurrentDataXY(m_dataX,m_dataY);//Отправляем сигнал с координатами клика
    qDebug()<<"x "<<m_dataX<<"y "<<m_dataY;


}

void Main2DWindow::finishPolygonCreation()
{
    disconnect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));
    flagPolygonIsCreated = true;
    flagDoubleClicked = true;
    setCursor(QCursor(Qt::ArrowCursor));
    this->setToolTip("");
    drawLine(polygonArr.last().last().x(), polygonArr.last().last().y(), polygonArr.last().first().x(),  polygonArr.last().first().y() );

    QImage mask = maskFromPolygons(polygonArr);
    QCPItemPixmap *pixItem = new QCPItemPixmap(ui->customPlot);
    QPixmap alphaImage(QPixmap::fromImage(mask));
    pixItem->setPixmap(alphaImage);
    ui->customPlot->addLayer("polygon");
    ui->customPlot->setCurrentLayer("polygon");
    pixItem->setScaled(true);
    ui->customPlot->addItem(pixItem);
    pixItem->topLeft->setCoords(0,0);
    pixItem->bottomRight->setCoords(rows-1,cols-1);
    pixItem->setClipToAxisRect(true);
    pixItem->setClipAxisRect(ui->customPlot->axisRect());
}

QImage Main2DWindow::maskFromPolygons(QVector<QPolygon> polygonArr)
{

    QElapsedTimer timer;
    timer.start();

    QImage mask(rows,cols,QImage::Format_ARGB32);
    mask.fill(qRgba(0, 0, 0, 0));
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            foreach(QPolygon polygon, polygonArr)
            {
                if(polygon.containsPoint(QPoint(i,j),Qt::OddEvenFill))
                    mask.setPixel(i,j,qRgba(255, 0, 0, 255));
            }
        }
    }
    qDebug()<<"create bit picture"<<timer.elapsed();
    return mask;
}
void Main2DWindow::mouseDblClickOnColorMap( QMouseEvent *e)
{
    if (!flagPolygonIsCreated)
        finishPolygonCreation();
    qDebug()<<"2x clicked";

}

void Main2DWindow::mouseMoveOnColorMap(QMouseEvent *e)
{
    int x = this->ui->customPlot->xAxis->pixelToCoord(e->pos().x());
    int y = this->ui->customPlot->yAxis->pixelToCoord(e->pos().y());
    int chan = ui->listWidget->currentRow();
    if (x >= 0 && x < rows && y >= 0 && y < cols)
    {
        qint16 bright = data[chan][x * cols + y];
        pStatusBarLabel->setText("X: " + QString().setNum(x) + "    Y: " + QString().setNum(y) + "    Значение:" + QString().setNum(bright));
    }
    else
        pStatusBarLabel->setText("");
}

void Main2DWindow::createMenus()
{
    pContextMenu = new QMenu();
    pContextMenu->setStyleSheet("border: 0px solid black;");
    pPlotAction = new QAction(QIcon(":/IconsCube/iconsCube/Plot.ico"),"Спектр",this);
    pPlotLineAction = new QAction(QIcon(":/IconsCube/iconsCube/PlotterLogo.ico"),"Спектральный срез", this);
    pPlotHistAction = new QAction("Гистограмма",this);
    pSelectAreaAction = new QAction(QIcon(":/IconsCube/iconsCube/polygon.png"), "Выбрать область",this);
    pContextMenu->addAction(pPlotAction);
    pContextMenu->addAction(pPlotLineAction);
    pContextMenu->addAction(pPlotHistAction);
    pContextMenu->addAction(pSelectAreaAction);
    connect(pPlotAction,SIGNAL(triggered()),SLOT(prepareToPlotSpectr()));
    connect(pPlotHistAction,SIGNAL(triggered()),SLOT(prepareToHist()));
    connect(pPlotLineAction,SIGNAL(triggered()),SLOT(createLinePlotterSlot()));
    connect(pSelectAreaAction,SIGNAL(triggered()),SLOT(createPolygonSlot()));
}

void Main2DWindow::drawLine(uint x1, uint y1, uint x2, uint y2)
{
    QCPItemLine *line = new QCPItemLine(ui->customPlot);
    line->start->setCoords(x1,y1);
    line->end->setCoords(x2,y2);
    line->setPen(QPen(Qt::red));
    ui->customPlot->addItem(line);
}

void Main2DWindow::setInitSliders(int chan)
{
    flagSlidersEnabledForSlots = false;
    int min =  32767;
    int max = -32767;
    qint16 *dataTemp = new qint16[rows*cols];
    for (int j = 0; j<rows*cols; ++j)
        dataTemp[j]=data[chan][j];
    QElapsedTimer timer3;
    timer3.start();
    for (int i = 0; i < rows*cols; ++i)
    {
       if(dataTemp[i] < min)
           min = dataTemp[i];
       if(dataTemp[i] > max)
           max = dataTemp[i];
    }
    qDebug()<<"нахождение мин макс"<<timer3.elapsed()<< " мс";

    ui->SliderContrastMin->setMinimum(min);
    ui->SliderContrastMin->setMaximum(max);
    ui->SliderContrastMin->setValue(ChnlLimits[chan][0]);
    ui->SliderContrastMax->setMinimum(min);
    ui->SliderContrastMax->setMaximum(max);
    ui->SliderContrastMax->setValue(ChnlLimits[chan][1]);
    delete[] dataTemp;
    flagSlidersEnabledForSlots = true;
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
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));
    //emit flagsToolTip(globalPos,"выберите начальную точку");
    connect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(startIsClicked(uint,uint)));
    pContextMenu->hide();
    this->setToolTip(strForLineHelp);

}

void Main2DWindow::createPolygonSlot()
{
    QString strForLineHelp = "Выберите точку; двойной щелчок для завершения";
    this->setToolTip(strForLineHelp);
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));   
    QPolygon polygon;
    polygonArr.append(polygon);
    flagPolygonIsCreated = false;
    connect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));

}

void Main2DWindow::addPolygonPoint(uint x,uint y)
{
    //раньше была проверка на flagPolygonIsCreated, но уже не надо
    if (polygonArr.last().size() > 0)
        drawLine(polygonArr.last().last().x(), polygonArr.last().last().y(), x, y );
    polygonArr.last().append(QPoint(x,y));


}
void Main2DWindow::startIsClicked(uint dataX, uint dataY)
{
    linePlotterIsActive = true;
    m_x1 = dataX;
    m_y1 = dataY;
    m_z1 = ui->listWidget->currentRow();
    //emit flagsToolTip(globalPos,"выберите конечную точку");
    QString strForLineHelp = "выберите конечную точку";
    this->setToolTip(strForLineHelp);
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/finish_flag.png"),10,29));
    disconnect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(startIsClicked(uint,uint)));
    connect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(finishIsClicked(uint,uint)));
}
void Main2DWindow::finishIsClicked(uint dataX, uint dataY)
{
    linePlotterIsActive = false;
    m_x2 = dataX;
    m_y2 = dataY;
    m_z2 = m_z1; //ui->listWidget->currentRow();
    QString strForLineHelp = "";
    setCursor(Qt::ArrowCursor);
    //emit signalPlotAlongLine(m_x1, m_x2, m_y1, m_y2, m_z1, m_z2); //так было в 3d кубе
    plotAlongLine(m_x1, m_x2, m_y1, m_y2, m_z1, m_z2);
    disconnect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(startIsClicked(uint,uint)));
    disconnect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(finishIsClicked(uint,uint)));
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
    connect(hist,SIGNAL(bordersSelected(int,int)),SLOT(contrastImage(int,int)));

}

void Main2DWindow::contextMenuRequest(QPoint point)
{

    pContextMenu->exec(ui->customPlot->mapToGlobal(point));//centralwidget->viewport()->mapToGlobal(point));

}







