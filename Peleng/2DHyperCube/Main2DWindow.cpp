#include "Main2DWindow.h"
#include "ui_Main2DWindow.h"
#include "../Library/GenericExc.h"
#include <QMessageBox>
#include "../Library/saveENVI.h"
#include "../Library/Spectr.h"

int cmp2(const void *a, const void *b);


Main2DWindow::Main2DWindow(HyperCube* cube, Attributes *attr, QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::Main2DWindow)
    , firstWindowPlotter(true)
    , linePlotterIsActive(false)
    , m_dataX(0)
    , m_dataY(0)
    , m_interplolate(false)
    , flagSlidersEnabledForSlots(false)
    , flagPolygonIsCreated(true)
    , flagDoubleClicked(false)
    , m_pCube(cube)
    , m_attributes(attr)
    , flagGetTempChannelFromCube(true)

{
    setAttribute(Qt::WA_DeleteOnClose, true);
    if( m_attributes->GetPointsList().size() > 0)

        m_initChanel = m_attributes->GetPointsList().at(0).z;
    else
        m_initChanel = 0;

    ui->setupUi(this);

    pContextMenu = 0;
    pPlotAction = 0;
    pPlotHistAction = 0;
    pPlotLineAction = 0;
    m_filters = 0;
    m_medianFilter = 0;
    m_actionMedian3 = 0;
    m_actionMedian5 = 0;
    m_actionMedian7 = 0;
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
    setHyperCube(cube);
    initArrChanLimits();
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(updateViewchan(int))); //коннектим здесь (до fillChanList), чтобы не было двух коннектов одного сигнала
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(setInitSliders(int)));
    fillChanList();
    polyMngr = new PolygonManager(m_pCube,m_attributes,ui->customPlot,this);
    createMenus();
    if (m_attributes->GetPointsList().size())
    {
        setInitChanel(m_attributes->GetPointsList().at(0).z);
//        setTempChannel(data[m_initChanel]);
        QVector<double> tempCh;
        m_pCube->GetDataChannel(m_initChanel,tempCh);
        setTempChannel(tempCh);

    }
    connect(ui->actionInterpolation,SIGNAL(toggled(bool)),SLOT(toggledActionInterpolation(bool)));
    connect(ui->customPlot,SIGNAL(customContextMenuRequested(QPoint)),SLOT(contextMenuRequest(QPoint)));
    connect(ui->customPlot,SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseMoveOnColorMap(QMouseEvent*)));
    connect(ui->customPlot,SIGNAL(mouseDoubleClick(QMouseEvent*)),polyMngr,SLOT(mouseDblClickOnParentColorMap(QMouseEvent*)));//перенесено в полиМенеджер

    connect(ui->customPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(mousePressOnColorMap(QMouseEvent*)));
    ui->listWidget->setCurrentRow(m_initChanel);


    connect(ui->SliderContrastMin,SIGNAL(valueChanged(int)),SLOT(leftBorderContrast(int)));
    connect(ui->SliderContrastMax,SIGNAL(valueChanged(int)),SLOT(rightBorderContrast(int)));
    connect(ui->polygonTool,SIGNAL(triggered()),SLOT(polygonTool()));

    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));


    emit  ui->listWidget->currentRowChanged(m_initChanel);

//    ui->frameCustomPlot->resize(this->size()); // чтобы избавиться от бага с очень маленьким размером фрейма
//    QResizeEvent *e = new QResizeEvent(this->size(),this->size());
//    resizeEvent(e);
//    delete e;
    m_needToUpdate = false;
    m_canDelete = true;
    connectionsOfPlugins();
}

Main2DWindow::~Main2DWindow()
{
    if (pContextMenu) delete pContextMenu;
    if (m_medianFilter) delete m_medianFilter;
    if (m_filters) delete m_filters;

    for (int i = 0; i < chnls; i++)
    {
        delete [] ChnlLimits[i];
    }
    delete [] ChnlLimits;
    delete[] m_tempChanel;
    delete ui;
    delete polyMngr;
}

void Main2DWindow::resizeEvent(QResizeEvent *e)
{
    QSize framesize =  ui->frameCustomPlot->size();
    double kFrame = (double)framesize.width() / (double)framesize.height();
    double RowsToCols = (double)rows / (double)cols;
    if(RowsToCols > 1)
    {
        //ui->customPlot->setFixedSize(framesize.width()*0.95 , framesize.width() / RowsToCols*0.95);
        //ui->customPlot->setMinimumHeight(framesize.width() / RowsToCols*0.95);
        if (kFrame > RowsToCols)
        {
            ui->customPlot->resize(framesize.width() / (kFrame/RowsToCols) , framesize.width() / (kFrame/RowsToCols) / RowsToCols);
            ui->customPlot->move(framesize.width()/2 - ui->customPlot->width()/2,framesize.height()/2 - ui->customPlot->height()/2 );
        }
        else
        {
            ui->customPlot->resize(framesize.width() , framesize.width() / RowsToCols);
            ui->customPlot->move(0,framesize.height()/2 - ui->customPlot->height()/2 );
        }
    }
    else
    {
        if (kFrame < RowsToCols)
        {
            ui->customPlot->resize(framesize.height() * RowsToCols * (kFrame/RowsToCols), framesize.height() * (kFrame/RowsToCols));
            ui->customPlot->move(framesize.width()/2 - ui->customPlot->width()/2,framesize.height()/2 - ui->customPlot->height()/2 );
        }
        else
        {
            ui->customPlot->resize(framesize.height() * RowsToCols, framesize.height());
            ui->customPlot->move(framesize.width()/2 - ui->customPlot->width()/2,0);
        }
    }
}

void Main2DWindow::closeEvent(QCloseEvent *e)
{
    if (m_canDelete)
    {
        emit CloseWindow();
    } else
    {
        QMessageBox::information(this, "Закрытие окна", "Невозможно закрыть окно программы! \nДождитесь окончания обработки данных");
        e->ignore();
    }

}
void Main2DWindow::updateData()
{
    ui->listWidget->setCurrentRow(m_initChanel);
    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));
    for (int i = 0; i < chnls; i++)
    {
        delete [] ChnlLimits[i];
    }
    delete [] ChnlLimits;
    initArrChanLimits();
    fillChanList();
    int minCMap, maxCMap;
    findMinMaxforColorMap(minCMap, maxCMap);
    drawHeatMap(minCMap, maxCMap);
}

void Main2DWindow::dataCubeResize()
{
    for (int i = 0; i < chnls; i++)
    {
        delete [] ChnlLimits[i];
    }
    delete [] ChnlLimits;
    delete[] m_tempChanel;
    setHyperCube(m_pCube);
    polyMngr->setRows(rows);
    polyMngr->setCols(cols);
    initArrChanLimits();

    fillChanList();
    //setInitSliders(0);
    ui->listWidget->setCurrentRow(0);
    ui->listWidget->item(0)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(0));
    emit  ui->listWidget->currentRowChanged(0);

    QResizeEvent* e;
    this->resizeEvent(e);
}

void Main2DWindow::connectionsOfPlugins()
{
    connect(m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), pContextMenu, SLOT(setEnabled(bool)));
    connect(m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), ui->menubar, SLOT(setEnabled(bool)));
    connect(m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), ui->listWidget, SLOT(setEnabled(bool)));
    connect(m_attributes->GetAvailablePlugins().value("Hist UI")->GetObjectPointer(), SIGNAL(ReplotSignal(Attributes*)), this, SLOT(plotFromAttributes(Attributes*)));
    connect (m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(FinishOperation(bool)), this, SLOT(needToUpdate(bool)));
    connect (m_attributes->GetAvailablePlugins().value("3DCube UI")->GetObjectPointer(), SIGNAL(StartOperation(bool)), pContextMenu, SLOT(setEnabled(bool)));
    connect (m_attributes->GetAvailablePlugins().value("3DCube UI")->GetObjectPointer(), SIGNAL(StartOperation(bool)), ui->menubar, SLOT(setEnabled(bool)));
    connect (m_attributes->GetAvailablePlugins().value("3DCube UI")->GetObjectPointer(), SIGNAL(StartOperation(bool)), ui->listWidget, SLOT(setEnabled(bool)));

    connect (m_attributes->GetAvailablePlugins().value("3DCube UI")->GetObjectPointer(), SIGNAL(FinishOperation(bool)), this, SLOT(needToResize(bool)));
}

void Main2DWindow::plotFromAttributes(Attributes *attr)
{

    int minCMap, maxCMap;
    findMinMaxforColorMap(minCMap, maxCMap,0,1);
    drawHeatMap(minCMap, maxCMap);
    qDebug()<<minCMap<<" max:"<<maxCMap;
    disconnect(ui->SliderContrastMin,SIGNAL(valueChanged(int)),this,SLOT(leftBorderContrast(int)));
    disconnect(ui->SliderContrastMax,SIGNAL(valueChanged(int)),this,SLOT(rightBorderContrast(int)));
    ui->SliderContrastMin->setValue(minCMap);
    ui->SliderContrastMax->setValue(maxCMap);
    connect(ui->SliderContrastMin,SIGNAL(valueChanged(int)),SLOT(leftBorderContrast(int)));
    connect(ui->SliderContrastMax,SIGNAL(valueChanged(int)),SLOT(rightBorderContrast(int)));
}

void Main2DWindow::needToUpdate(bool res)
{
    pContextMenu->setEnabled(true);
    ui->menubar->setEnabled(true);
    ui->listWidget->setEnabled(true);
    QCoreApplication::processEvents();
    //this->setEnabled(true);
    if (res)
    {
        // кнопка
        updateData();
        m_needToUpdate = true;
    }
}

void Main2DWindow::needToResize(bool res)
{
    pContextMenu->setEnabled(res);
    ui->menubar->setEnabled(res);
    ui->listWidget->setEnabled(res);
    dataCubeResize();

}

void Main2DWindow::setInitChanel(u::uint32 initChanel)
{
    m_initChanel = initChanel;
    m_pCube->GetDataChannel(m_initChanel,m_tempChanel);
    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));
    //или update
//    int minCMap, maxCMap;
//    findMinMaxforColorMap(minCMap, maxCMap,0.04, 0.98);
//    drawHeatMap(minCMap, maxCMap);
}

void Main2DWindow::setHyperCube(HyperCube *ptrCube)
{
    m_pCube = ptrCube;
    rows = m_pCube->GetLines();
    cols = m_pCube->GetColumns();
    chnls = m_pCube->GetCountofChannels();
    m_tempChanel = new double[rows*cols];
    colorMap->data()->setSize(rows, cols);
    colorMap->data()->setRange(QCPRange(0, rows-1), QCPRange(0, cols-1));
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
    ui->customPlot->axisRect()->setMargins(QMargins(0,0,0,0));// -1 устраняет баг с полосой белых пикселей при 0 (0,0,0,-1) //PS: убрал, не надо
    colorMap->setKeyAxis(ui->customPlot->xAxis);
    colorMap->setValueAxis(ui->customPlot->yAxis);
    ui->customPlot->addPlottable(colorMap);
    ui->customPlot->setInteraction(QCP::iRangeDrag , true);
    ui->customPlot->setInteraction(QCP::iRangeZoom  , true);
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->rescaleDataRange(true);
}

//void Main2DWindow::setTempChannel(qint16* chanData)
void Main2DWindow::setTempChannel(QVector<double> chanData)
{
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            m_tempChanel[i*cols+j] = chanData.at(i*cols+j);//(double)chanData[i*cols+j];
    m_attributes->SetTempChanel(m_tempChanel);
}

void Main2DWindow::fillChanList()
{

    disconnect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(updateViewchan(int)));
    disconnect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(setInitSliders(int)));
    QList<double> list = m_pCube->GetListOfChannels();
    int num = 0;
    foreach(double i , list) {
        ui->listWidget->insertItem(num,QString("%1 - %2 нм").arg(num).arg(i));
        num++;
    }
    ui->listWidget->clearSelection();
    QApplication::processEvents();
    int nRows = ui->listWidget->count();
    for(int i = nRows-1; i >num-1 ; --i)
    {
        delete ui->listWidget->item(i);
    }
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(updateViewchan(int)));
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(setInitSliders(int)));

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

void Main2DWindow::drawHeatMap(int minCMap, int maxCMap)
{
    for (int x=0; x < rows; ++x) {
        for (int y=0; y < cols; ++y) {
            colorMap->data()->setCell(x, y,m_tempChanel[x * cols + y] );
        }
    }
    ui->customPlot->rescaleAxes();
    colorMap->setInterpolate(m_interplolate);
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    ui->customPlot->replot();
}





void Main2DWindow::findMinMaxforColorMap(int &minCMap, int &maxCMap,float thresholdLow,float thresholdHigh)
//thresholdLow = 0.02 (первые 2% игнорируются), thresholdHigh = 0.98
{
    minCMap =  32767;
    maxCMap = -32767;
    double *dataTemp = new double[rows*cols];
    try {
         memcpy(dataTemp, m_tempChanel, rows*cols*sizeof(double));
    } catch (...) {
        throw GenericExc("Неудалось копировать в темповый канал", -1);
    }
    QElapsedTimer timer3;
    timer3.start();
    qsort(dataTemp,cols*rows,sizeof(double),cmp2);
    qDebug()<<"сортировка"<<timer3.elapsed()<< " мс";
    minCMap = dataTemp[int((rows*cols-1)*thresholdLow)];
    maxCMap = dataTemp[int((rows*cols-1)*thresholdHigh)];
    delete[] dataTemp;
}

int cmp2(const void *a, const void *b)
{
    const double *pa = (const double*)a;
    const double *pb = (const double*)b;
    if (*pa < *pb)
        return -1;
    else if (*pa > *pb)
        return +1;
    else
        return 0;
}

void Main2DWindow::updateViewchan(int chan)
{
    if(flagGetTempChannelFromCube)
    {
        QVector<double> tempCh;
        m_pCube->GetDataChannel(chan,tempCh);
        setTempChannel(tempCh);
//        setTempChannel(data[chan]);//data[chan]
    }
    if(ChnlLimits[chan][0] == -32767 || ChnlLimits[chan][1] == -32767 )
    {
        int minCMap, maxCMap;
        findMinMaxforColorMap(minCMap, maxCMap,0.04, 0.98);

        ChnlLimits[chan][0] = minCMap;
        ChnlLimits[chan][1] = maxCMap;
    }
//    for(auto line:m_lines)
//        delete line;
    foreach(QCPItemLine* line, m_lines)
    {
        ui->customPlot->removeItem(line);
    }
    m_lines.clear();

    drawHeatMap(ChnlLimits[chan][0], ChnlLimits[chan][1]);
}



void Main2DWindow::leftBorderContrast(int left)
{
    if (flagSlidersEnabledForSlots)
    {
        int chan = ui->listWidget->currentRow();
        ChnlLimits[chan][0] = left;
        drawHeatMap(ChnlLimits[chan][0], ChnlLimits[chan][1]);
    }
}

void Main2DWindow::rightBorderContrast(int right)
{
    if (flagSlidersEnabledForSlots)
    {
        int chan = ui->listWidget->currentRow();
        ChnlLimits[chan][1] = right;
        drawHeatMap(ChnlLimits[chan][0], ChnlLimits[chan][1]);
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
    if(flagDoubleClicked) // потом брать этот флаг из мэнеджера. Флаг нужен, чтобы повторно не добавлялась одна и та же точка
    {
        flagDoubleClicked  = false;
        return;
    }
    double x = this->ui->customPlot->xAxis->pixelToCoord(e->pos().x());
    double y = this->ui->customPlot->yAxis->pixelToCoord(e->pos().y());
    if(x<0)
        m_dataX = 0;
    else if(x>rows)
        m_dataX = rows-1;
    else
        m_dataX = qRound(x);

    if(y<0)
        m_dataY = 0;
    else if(y>cols)
        m_dataY = cols-1;
    else
        m_dataY = qRound(y);

    emit signalCurrentDataXY(m_dataX,m_dataY);//Отправляем сигнал с координатами клика

    //if(x>=0 && x <= rows-1 && y >= 0 && y <= cols-1)
        emit signalDoubleCordsClicked(x,y);
}


void Main2DWindow::mouseMoveOnColorMap(QMouseEvent *e)
{
    double x = this->ui->customPlot->xAxis->pixelToCoord(e->pos().x());
    double y = this->ui->customPlot->yAxis->pixelToCoord(e->pos().y());
    int xInt = qRound(x);
    int yInt = qRound(y);
    if (xInt >= 0 && xInt < rows && yInt >= 0 && yInt < cols)
    {
        double bright = m_tempChanel[xInt * cols + yInt];
        pStatusBarLabel->setText("X: " + QString().setNum(xInt) + "    Y: " + QString().setNum(yInt) + "    Значение:" + QString().setNum(bright));
    }
    else
        pStatusBarLabel->setText("");
}

void Main2DWindow::toggledActionInterpolation(bool flag)
{
    m_interplolate = flag;
    updateViewchan(ui->listWidget->currentRow());
}

void Main2DWindow::createMenus()
{
    pContextMenu = new QMenu();
    pContextMenu->setStyleSheet("border: 0px solid black;");
    pPlotAction = new QAction(QIcon(":/IconsCube/iconsCube/Plot.png"),"Спектр",this);

    pPlotLineAction = new QAction(QIcon(":/IconsCube/iconsCube/Line Plotter.png"),"Пространственный срез", this);
    pPlotHistAction = new QAction(QIcon(":/icons2Dcube/icons/histogram.png"),"Гистограмма",this);
    pAddSpectr = new QAction(QIcon(":/IconsCube/iconsCube/CreateSpectr.png"),"Загрузить спектр",this);
    m_filters = new QMenu();
    m_filters->setStyleSheet("border: 0px solid black;");
    m_filters->setTitle("Фильтры");
    m_filters->setIcon(QIcon(":/IconsCube/iconsCube/NoiseRemover.png"));
    m_medianFilter = new QMenu();
    m_medianFilter->setStyleSheet("border: 0px solid black;");
    m_medianFilter->setTitle("Медианный фильтр");

    m_actionMedian3 = new QAction("3x3", this);
    m_actionMedian5 = new QAction("5x5", this);
    m_actionMedian7 = new QAction("7x7", this);;

    if (m_attributes->GetAvailablePlugins().contains("Spectr UI"))
    {
        pContextMenu->addAction(pPlotAction);
        connect(pPlotAction,SIGNAL(triggered()),SLOT(prepareToPlotSpectr()));
        //connect(this,SIGNAL(sendXYZ(uint,uint,uint)),SLOT(plotSpectr(uint,uint,uint) ));
    }
    if (m_attributes->GetAvailablePlugins().contains("Hist UI"))
    {
        pContextMenu->addAction(pPlotHistAction);
        connect(pPlotHistAction,SIGNAL(triggered()),SLOT(prepareToHist()));
    }
    if (m_attributes->GetAvailablePlugins().contains("Line Plotter UI"))
    {
        pContextMenu->addAction(pPlotLineAction);
        connect(pPlotLineAction,SIGNAL(triggered()),SLOT(createLinePlotterSlot()));

    }
    if (m_attributes->GetAvailablePlugins().contains("SpectralLib UI"))
    {
        pContextMenu->addAction(pAddSpectr);
        connect(pAddSpectr,SIGNAL(triggered()),SLOT(addSpectr()));
    }
	if (m_attributes->GetAvailablePlugins().contains("Noise Remover"))
    {
        m_medianFilter->addAction(m_actionMedian3);
        m_medianFilter->addAction(m_actionMedian5);
        m_medianFilter->addAction(m_actionMedian7);
        m_filters->addMenu(m_medianFilter);
        pContextMenu->addMenu(m_filters);
        connect(m_actionMedian3, SIGNAL(triggered()), this, SLOT(OnActionMedian3Triggered()));
        connect(m_actionMedian5, SIGNAL(triggered()), this, SLOT(OnActionMedian5Triggered()));
        connect(m_actionMedian7, SIGNAL(triggered()), this, SLOT(OnActionMedian7Triggered()));
    }
    if (m_attributes->GetAvailablePlugins().contains("SpectralDistance"))
    {
        pContextMenu->addAction(QIcon(":/IconsCube/iconsCube/distance.png"),
                                "Сравнить со спектральными кривыми", this, SLOT(ActionSpectralDistanceToogled()));
    }
}



void Main2DWindow::setInitSliders(int chan)
{
    flagSlidersEnabledForSlots = false;
    int min =  32767;
    int max = -32767;

    for (int i = 0; i < rows*cols; ++i)
    {
       if(m_tempChanel[i] < min)
           min = (int)m_tempChanel[i];
       if(m_tempChanel[i] > max)
           max = (int)m_tempChanel[i];
    }
    ui->SliderContrastMin->setMinimum(min);
    ui->SliderContrastMin->setMaximum(max);
    ui->SliderContrastMin->setValue(ChnlLimits[chan][0]);
    ui->SliderContrastMax->setMinimum(min);
    ui->SliderContrastMax->setMaximum(max);
    ui->SliderContrastMax->setValue(ChnlLimits[chan][1]);
    flagSlidersEnabledForSlots = true;
}

void Main2DWindow::polygonTool()
{
    polyMngr->show();
    polyMngr->raise();
    polyMngr->showNormal();
}

void Main2DWindow::OnActionMedian3Triggered()
{
    m_attributes->SetMaskPixelsCount(3);
    Noise();
}

void Main2DWindow::OnActionMedian5Triggered()
{
    m_attributes->SetMaskPixelsCount(5);
    Noise();
}

void Main2DWindow::OnActionMedian7Triggered()
{
    m_attributes->SetMaskPixelsCount(7);
    Noise();
}

void Main2DWindow::Noise()
{
    this->setWindowFlags ( Qt::CustomizeWindowHint | Qt::WindowTitleHint);//вариант 2
    this->show();
    m_canDelete = false;
    pStatusBarLabel->setText("Пожалуйта подождите");
    pContextMenu->setEnabled(false);
    ui->menubar->setEnabled(false);
    ui->listWidget->setEnabled(false);
    QCoreApplication::processEvents();
    m_attributes->ClearList();
    m_attributes->SetPoint(0,0, ui->listWidget->currentRow());
    m_attributes->SetNoiseAlg(Median2D);
    m_attributes->SetApplyToAllCube(false);
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_pCube, m_attributes);
    m_canDelete = true;
    this->setWindowFlags (this->windowFlags()  & ~Qt::CustomizeWindowHint &~Qt::WindowTitleHint);//вариант 2
    this->show();
    this->lower();

}

void Main2DWindow::plotSpectr(uint x, uint y)
{
    if (!m_needToUpdate)
    {
        Spectr* spectr = new Spectr(m_pCube, x, y);
        m_attributes->SetCurrentSpectr(spectr);
        m_attributes->SetCurrentTitle(spectr->GetTitle());
        m_attributes->SetExternalSpectrFlag(false);
        m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(m_pCube, m_attributes);
    } else
    {
        int answer = QMessageBox::question(this, "Обновление", "Необходимо обновить данные. Обновить?", "Да", "Нет", QString(), 0, 1);
        if (answer == 0)
        {
            updateData();
            m_needToUpdate = false;
        }
    }
}

void Main2DWindow::addSpectr()
{
    m_attributes->SetModeLib(1);
    m_attributes->SetExternalSpectrFlag(true);
    m_attributes->GetAvailablePlugins().value("SpectralLib UI")->Execute(m_pCube , m_attributes);
}

void Main2DWindow::prepareToPlotSpectr()
{
    plotSpectr(m_dataX,m_dataY);
}
void Main2DWindow::createLinePlotterSlot()
{
    linePlotterIsActive = true;
    QString strForLineHelp = "Выберите начальную точку";
    ui->customPlot->setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));
    connect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(startIsClicked(uint,uint)));
    pContextMenu->hide();
    this->setToolTip(strForLineHelp);
}



void Main2DWindow::startIsClicked(uint dataX, uint dataY)
{
    linePlotterIsActive = true;
    m_x1 = dataX;
    m_y1 = dataY;
    m_z1 = ui->listWidget->currentRow();
    QString strForLineHelp = "выберите конечную точку";
    this->setToolTip(strForLineHelp);
    ui->customPlot->setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/finish_flag.png"),10,29));
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
    ui->customPlot->setCursor(Qt::ArrowCursor);
    plotAlongLine(m_x1, m_x2, m_y1, m_y2, m_z1, m_z2);
    disconnect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(startIsClicked(uint,uint)));
    disconnect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(finishIsClicked(uint,uint)));
    this->setToolTip(strForLineHelp);

}

void Main2DWindow::plotAlongLine(uint x1, uint x2, uint y1, uint y2, uint z1, uint z2)
{
    if (!m_needToUpdate)
    { 

        try{
            m_attributes->ClearList();
            m_attributes->SetPoint(x1, y1, z1);
            m_attributes->SetPoint(x2, y2, z2);
            m_attributes->GetAvailablePlugins().value("Line Plotter UI")->Execute(m_pCube, m_attributes);
            drawLine(QPoint(x1,y1),QPoint(x2,y2));
        }catch(GenericExc e){
            QMessageBox::warning(this,"Предупреждение", e.GetWhat());
        }
    } else
    {
        int answer = QMessageBox::question(this, "Обновление", "Необходимо обновить данные. Обновить?", "Да", "Нет", QString(), 0, 1);
        if (answer == 0)
        {
            updateData();
            m_needToUpdate = false;
        }
    }
}

void Main2DWindow::drawLine(QPoint p1, QPoint p2)
{
    QCPItemLine *line = new QCPItemLine(ui->customPlot);
    line->start->setCoords(p1);
    line->end->setCoords(p2);
    line->setPen(QPen(Qt::red));
    ui->customPlot->addItem(line);
    ui->customPlot->replot();
    m_lines.append(line);
}

void Main2DWindow::prepareToHist()
{
    if (!m_needToUpdate)
    {
//        m_attributes->ClearList();
//        m_attributes->SetPoint(0, 0, ui->listWidget->currentRow());
        m_attributes->GetAvailablePlugins().value("Hist UI")->Execute(m_pCube, m_attributes);
    } else
    {
   /*     int answer = QMessageBox::question(this, "Обновление", "Необходимо обновить данные. Обновить?", "Да", "Нет", QString(), 0, 1);
        if (answer == 0)
        {
            updateData();
            m_needToUpdate = false;
        }*/
    }
}

void Main2DWindow::contextMenuRequest(QPoint point)
{

    pContextMenu->exec(ui->customPlot->mapToGlobal(point));//centralwidget->viewport()->mapToGlobal(point));

}


void Main2DWindow::ActionSpectralDistanceToogled()
{
    m_attributes->ClearList();
    m_attributes->SetPoint(m_dataX, m_dataY, 0);
    m_attributes->SetExternalSpectrFlag(false);
    m_attributes->GetAvailablePlugins().value("SpectralDistance")->Execute(m_pCube, m_attributes);
}

void Main2DWindow::on_action_saveENVI_triggered()
{
     saveENVI(m_pCube);
}
