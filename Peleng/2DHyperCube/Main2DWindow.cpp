#include "Main2DWindow.h"
#include "ui_Main2DWindow.h"

#include <QMessageBox>

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
    fillChanList();
    polyMngr = new PolygonManager(rows,cols,ui->customPlot,this);
    createMenus();
    if (m_attributes->GetPointsList().size())
    {
        setInitChanel(m_attributes->GetPointsList().at(0).z);
    }
    connect(ui->actionInterpolation,SIGNAL(toggled(bool)),SLOT(toggledActionInterpolation(bool)));
    connect(ui->customPlot,SIGNAL(customContextMenuRequested(QPoint)),SLOT(contextMenuRequest(QPoint)));
    connect(ui->customPlot,SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseMoveOnColorMap(QMouseEvent*)));
    connect(ui->customPlot,SIGNAL(mouseDoubleClick(QMouseEvent*)),polyMngr,SLOT(mouseDblClickOnParentColorMap(QMouseEvent*)));//перенесено в полиМенеджер

    connect(ui->customPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(mousePressOnColorMap(QMouseEvent*)));
    ui->listWidget->setCurrentRow(m_initChanel);
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(updateViewchan(int)));

    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(setInitSliders(int)));
    connect(ui->SliderContrastMin,SIGNAL(valueChanged(int)),SLOT(leftBorderContrast(int)));
    connect(ui->SliderContrastMax,SIGNAL(valueChanged(int)),SLOT(rightBorderContrast(int)));
    connect(ui->polygonTool,SIGNAL(triggered()),SLOT(polygonTool()));

    ui->listWidget->item(m_initChanel)->setSelected(true);
    ui->listWidget->setFocus();
    ui->listWidget->scrollToItem(ui->listWidget->item(m_initChanel));


    emit  ui->listWidget->currentRowChanged(m_initChanel);

    ui->frameCustomPlot->resize(this->size()); // чтобы избавиться от бага с очень маленьким размером фрейма


    //    QSize mainSize = this->size();
//    if(cols > rows) //rows/cols<1
//        this->resize(mainSize.width() * rows/cols , mainSize.height());
//    else
//        this->resize(mainSize.width(), mainSize.width() * rows / cols);
    //ui->customPlot->setMinimumSize(this->size().width() * 0.75 ,this->size().width() * 0.75* cols / rows);
    //ui->customPlot->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//loadMaskFromFile();// потом удалить отсюда!!!
    //проверить
  //  int minCMap, maxCMap;
   // findMinMaxforColorMap(m_initChanel,minCMap, maxCMap);
  //  drawHeatMap(m_initChanel,minCMap, maxCMap);
    m_needToUpdate = false;
    m_canDelete = true;
    connectionsOfPlugins();

}

Main2DWindow::~Main2DWindow()
{
    if (pContextMenu) delete pContextMenu;
    if (pPlotAction) delete pPlotAction;
    if (pPlotHistAction) delete pPlotHistAction;
    if (pPlotLineAction) delete pPlotLineAction;
    if (m_filters)
    {
        delete m_filters;
        delete m_medianFilter;
        delete m_actionMedian3;
        delete m_actionMedian5;
        delete m_actionMedian7;
    }
    for (int i = 0; i < chnls; i++)
    {
        delete [] ChnlLimits[i];
    }
    delete [] ChnlLimits;
    delete ui;
}


void Main2DWindow::resizeEvent(QResizeEvent *)
{
   // ui->customPlot->setMinimumWidth(1);
   // ui->customPlot->setFixedHeight( ui->customPlot->width()* cols / rows);
   // ui->customPlot->setFixedWidth(ui->customPlot->height()*  rows/cols);
    //ui->customPlot->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSize framesize =  ui->frameCustomPlot->size();
    double RowsToCols = (double)rows / (double)cols;
    if(RowsToCols > 1)
    {
        ui->customPlot->setFixedSize(framesize.width()*0.95 , framesize.width() / RowsToCols*0.95);
    }
        else
    {
        ui->customPlot->setFixedSize(framesize.height() * RowsToCols*0.95, framesize.height()*0.95);
    }
}

void Main2DWindow::closeEvent(QCloseEvent *e)
{
    if (m_canDelete)
    {
        emit CloseWindow(this);
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
    initArrChanLimits();
    fillChanList();
    int minCMap, maxCMap;
    findMinMaxforColorMap(m_initChanel,minCMap, maxCMap);
    drawHeatMap(m_initChanel,minCMap, maxCMap);
}

void Main2DWindow::connectionsOfPlugins()
{
    connect(m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), pContextMenu, SLOT(setEnabled(bool)));
    connect(m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), ui->menubar, SLOT(setEnabled(bool)));
    connect(m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(StartOperation(bool)), ui->listWidget, SLOT(setEnabled(bool)));
    connect (m_attributes->GetAvailablePlugins().value("Noise Remover")->GetObjectPointer(), SIGNAL(FinishOperation(bool)), this, SLOT(needToUpdate(bool)));
}

void Main2DWindow::needToUpdate(bool res)
{
    pContextMenu->setEnabled(true);
    ui->menubar->setEnabled(true);
    ui->listWidget->setEnabled(true);
    if (res)
    {
        // кнопка
        updateData();
        m_needToUpdate = true;


    }
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
}





void Main2DWindow::findMinMaxforColorMap(int chan, int &minCMap, int &maxCMap,float thresholdLow,float thresholdHigh)
//thresholdLow = 0.02 (первые 2% игнорируются), thresholdHigh = 0.98
{
    minCMap =  32767;
    maxCMap = -32767;
    qint16 *dataTemp = new qint16[rows*cols];

    m_pCube->GetDataChannel(chan, dataTemp);
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
    if(flagDoubleClicked) // потом брать этот флаг из мэнеджера. Флаг нужен, чтобы повторно не добавлялась одна и та же точка
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

/*void Main2DWindow::finishPolygonCreation()
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
//    ui->customPlot->addLayer("polygon");
//    ui->customPlot->setCurrentLayer("polygon");
    pixItem->setScaled(true,Qt::KeepAspectRatio,Qt::FastTransformation);
    ui->customPlot->addItem(pixItem);
    pixItem->topLeft->setCoords(0,0);
    pixItem->bottomRight->setCoords(rows-1,cols-1);
    pixItem->setClipToAxisRect(true);
    pixItem->setClipAxisRect(ui->customPlot->axisRect());
    ui->customPlot->replot();


}*/

/*QImage Main2DWindow::maskFromPolygons(QVector<QPolygon> polygonArr)
{

    QElapsedTimer timer;
    timer.start();

    QImage mask(rows,cols,QImage::Format_ARGB32);
    mask.fill(qRgba(0, 0, 0, 0));
    QByteArray byteArr(rows*cols,0x00);
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            foreach(QPolygon polygon, polygonArr)
            {
                if(polygon.containsPoint(QPoint(i,j),Qt::OddEvenFill))
                 {
                    byteArr[i*cols+j] = 0x01;
                    mask.setPixel(i,j,qRgba(255, 0, 0, 255));
                }
            }
        }
    }
    qDebug()<<"create ARGB32 picture"<<timer.elapsed();

    // -------------------------------Тест, начало

    QBitmap bitMap(rows,cols);
    bitMap.fill(Qt::color0); //прозрачный


    QFile file("D:/selected.area");
    if(file.open(QIODevice::WriteOnly))
    {   file.write(byteArr);
        file.close();
    }

    //--------------------------------- Тест, конец

    return mask;
}*/
/*void Main2DWindow::mouseDblClickOnColorMap( QMouseEvent *e)
{
    if (!flagPolygonIsCreated)
        //finishPolygonCreation();
        polyMngr->finishPolygonCreation();
    qDebug()<<"2x clicked";

}*/

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
    pPlotHistAction = new QAction(QIcon("qrc:/icons2Dcube/icons/Heat Map-50.png"),"Гистограмма",this);
    pAddSpectr = new QAction(QIcon(":/IconsCube/iconsCube/CreateSpectr.png"),"Загрузить спектр",this);
    m_filters = new QMenu();
    m_filters->setStyleSheet("border: 0px solid black;");
    m_filters->setTitle("Фильтры");
    m_filters->setIcon(QIcon(":/icons/NoiseRemover.png"));
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
    pSelectAreaAction = new QAction(QIcon(":/IconsCube/iconsCube/polygon.png"), "Выбрать область",this);
	pContextMenu->addAction(pSelectAreaAction);
    //connect(pSelectAreaAction,SIGNAL(triggered()),SLOT(createPolygonSlot()));//удалить строчку
    connect(pSelectAreaAction,SIGNAL(triggered()),polyMngr,SLOT(createPolygonSlot()));
}

/*void Main2DWindow::drawLine(uint x1, uint y1, uint x2, uint y2)
{
    QCPItemLine *line = new QCPItemLine(ui->customPlot);
    line->start->setCoords(x1,y1);
    line->end->setCoords(x2,y2);
    line->setPen(QPen(Qt::red));
    ui->customPlot->addItem(line);
    ui->customPlot->replot();

}*/

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

void Main2DWindow::polygonTool()
{
    polyMngr->show();    
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
    m_attributes->ClearList();
    m_attributes->SetPoint(0,0, ui->listWidget->currentRow());
    m_attributes->SetNoiseAlg(Median2D);
    m_attributes->SetApplyToAllCube(false);
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_pCube, m_attributes);
}

void Main2DWindow::plotSpectr(uint x, uint y)
{
    if (!m_needToUpdate)
    {
        m_attributes->ClearList();
        m_attributes->SetPoint(x, y, 0);
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
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));
    //emit flagsToolTip(globalPos,"выберите начальную точку");
    connect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(startIsClicked(uint,uint)));
    pContextMenu->hide();
    this->setToolTip(strForLineHelp);


}

/*void Main2DWindow::createPolygonSlot()
{
    QString strForLineHelp = "Выберите точку; двойной щелчок для завершения";
    this->setToolTip(strForLineHelp);
    setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));   
    QPolygon polygon;
    polygonArr.append(polygon);
    flagPolygonIsCreated = false;
    connect(this,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));

}*/

/*void Main2DWindow::addPolygonPoint(uint x,uint y)
{
    //раньше была проверка на flagPolygonIsCreated, но уже не надо
    if (polygonArr.last().size() > 0)
        drawLine(polygonArr.last().last().x(), polygonArr.last().last().y(), x, y );
    polygonArr.last().append(QPoint(x,y));


}*/

/*void Main2DWindow::loadMaskFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),"*.area");
    QFile file(fileName);
    QByteArray byteArr;
    if(file.open(QIODevice::ReadOnly))
    {
        byteArr = file.readAll(); // добавить в Try catch
        file.close();
        // Идет копипаст, todo
        QImage mask(rows,cols,QImage::Format_ARGB32);
        mask.fill(qRgba(0, 0, 0, 0));
        QElapsedTimer timer;
        timer.start();
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                if(byteArr[i*cols+j] == (char)0x01) //проверить (char)
                     mask.setPixel(i,j,qRgba(0, 0, 255, 150));
            }
        }
        qDebug()<<"create image from byte array: "<<timer.elapsed();

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
    else
        qDebug()<<fileName<<" isn't opened";


}*/

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
    if (!m_needToUpdate)
    {
        m_attributes->ClearList();
        m_attributes->SetPoint(x1, y1, z1);
        m_attributes->SetPoint(x2, y2, z2);
        m_attributes->GetAvailablePlugins().value("Line Plotter UI")->Execute(m_pCube, m_attributes);
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

void Main2DWindow::prepareToHist()
{
    if (!m_needToUpdate)
    {
        m_attributes->ClearList();
        m_attributes->SetPoint(0, 0, ui->listWidget->currentRow());
        m_attributes->GetAvailablePlugins().value("Hist UI")->Execute(m_pCube, m_attributes);
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

void Main2DWindow::contextMenuRequest(QPoint point)
{

    pContextMenu->exec(ui->customPlot->mapToGlobal(point));//centralwidget->viewport()->mapToGlobal(point));

}







