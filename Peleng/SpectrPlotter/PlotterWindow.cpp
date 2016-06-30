#include "PlotterWindow.h"
#include "ui_PlotterWindow.h"

#include <QtAlgorithms>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include "../Library/structures.h"

PlotterWindow::PlotterWindow(HyperCube* cube, Attributes* attr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlotterWindow)
    , minY(70000)
    , maxY(-10000)
    , m_cube(cube)
    , m_attributes(attr)

{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowIcon(QIcon(":/logo/IconsPlotter/Plot.png"));
    ui->setupUi(this);
    m_customPlot = (QCustomPlot*) ui->PlotWidget;
    m_hold = false;
    m_valuesFlag = false;
    m_pointsFlag = false;
    initSize = size();
    textValues = new QCPItemText(m_customPlot);
    vertLine = new QCPItemStraightLine(m_customPlot);
    horizLine = new QCPItemStraightLine(m_customPlot);
    textValues->setVisible(false); // т.к. есть баг, что при создании item'ов они сразу отрисовываются в (0,0)
    vertLine->setVisible(false);
    horizLine->setVisible(false);
    QPropertyAnimation* panim = new QPropertyAnimation(this, "windowOpacity");
    panim->setDuration(300);
    panim->setStartValue(0);
    panim->setEndValue(1);
    panim->setEasingCurve(QEasingCurve::InCirc);
    panim->start(QAbstractAnimation::DeleteWhenStopped);
    if (m_attributes->GetAvailablePlugins().contains("SpectralLib UI"))
    {
        ui->menuSpectrum->addAction("Загрузить спектр", this, SLOT(AddSpectr()));
    }
    if (m_attributes->GetExternalSpectrFlag())
    {
        //m_descriptionExternalSpectr.append(m_attributes->GetCurrentSpectr()->GetTitle());
    }
    else
         ui->menuSpectrum->removeAction(ui->actionInterplol);

    connect(m_customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),  SLOT(graphClicked(QCPAbstractPlottable*)));
    m_customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(CompareSpectrWithCube()));
    connect(m_customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    connect(m_customPlot, SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(mouseMoveRequest(QMouseEvent*)));
    connect(ui->actionValues, SIGNAL(toggled(bool)),this,SLOT(onActionValues(bool)));
    connect(ui->actionPoints, SIGNAL(toggled(bool)),this,SLOT(onActionPoints(bool)));

    leftLine = new QCPItemStraightLine(m_customPlot); // Удаляется при удалении ui->beforeCustomPlot
    m_customPlot->addItem(leftLine);
    rightLine = new QCPItemStraightLine(m_customPlot);// Удаляется при удалении ui->beforeCustomPlot
    m_customPlot->addItem(rightLine);

    leftLine->setVisible(false);
    rightLine->setVisible(false);

}

PlotterWindow::~PlotterWindow()
{
    m_xArr.clear();
    m_yArr.clear();
    delete ui;
}

void PlotterWindow::closeEvent(QCloseEvent *) {
    emit closePlotterWindow(this);
}

void PlotterWindow::NoiseMedianAlgExecute()
{
    bool oldHold = m_hold;
    m_hold = true;
    m_attributes->SetNoiseAlg(Median1D);
    m_attributes->SetApplyToAllCube(false);
    m_attributes->SetCurrentSpectr(m_selectedSpectr);
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_cube, m_attributes);
    m_hold = oldHold;
}

void PlotterWindow::AddSpectr()
{
    m_attributes->SetModeLib(1);
    m_attributes->SetExternalSpectrFlag(true);
    m_attributes->GetAvailablePlugins().value("SpectralLib UI")->Execute(m_cube , m_attributes);
}

void PlotterWindow::graphClicked(QCPAbstractPlottable * plottable)
{
    int index = m_customPlot->selectedGraphs().indexOf(dynamic_cast<QCPGraph*>(plottable));
    if (index > -1 && index < m_listSpectr.size())
    {
        m_selectedSpectr = m_listSpectr.at(index);
    }
}

void PlotterWindow::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    if (m_customPlot->selectedGraphs().size() > 0)
    {
        menu->addAction("Удалить выделенный график", this, SLOT(removeSelectedGraph()));
        menu->addAction("Оставить выделенный график",this,SLOT(removeAllExceptSelectedGraph()));
        menu->addAction("Интерполяция", this, SLOT(onActionInterplol()));
        menu->addAction("Нормировка", this, SLOT(ActionNormalization()));
        if (m_attributes->GetAvailablePlugins().contains("SpectralLib UI"))
        {
            menu->addAction("Сохранить в библиотеку",this,SLOT(on_actionSave_toggled()));
        }
        if (m_attributes->GetAvailablePlugins().contains("Noise Remover"))
        {
            QMenu* menuNoise = new QMenu("Фильтры", this);

            QMenu* menuNoiseMedian = new QMenu("Медианный фильтр", this);
            menuNoiseMedian->addAction("Маска: 3 пикселя", this, SLOT(ActionNoise3MedianToggled()));
            menuNoiseMedian->addAction("Маска: 5 пикселей", this, SLOT(ActionNoise5MedianToggled()));
            menuNoiseMedian->addAction("Маска: 7 пикселей", this, SLOT(ActionNoise7MedianToggled()));
            menuNoise->addMenu(menuNoiseMedian);

            QMenu* menuNoiseSavGolay = new QMenu("Савитского-Голау фильтр", this);

            QMenu* menuSavitskogoGolayDegreePoligons_2 = new QMenu("Полином 2-й степени", this);
            menuSavitskogoGolayDegreePoligons_2->addAction("Маска: 5 пикселей", this, SLOT(ActionNoiseSavitGolay2_5Toogled()));
            menuSavitskogoGolayDegreePoligons_2->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay2_7Toogled()));
            menuSavitskogoGolayDegreePoligons_2->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay2_9Toogled()));

            QMenu* menuSavitskogoGolayDegreePoligons_3 = new QMenu("Полином 3-й степени", this);
            menuSavitskogoGolayDegreePoligons_3->addAction("Маска: 5 пикселей", this, SLOT(ActionNoiseSavitGolay3_5Toogled()));
            menuSavitskogoGolayDegreePoligons_3->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay3_7Toogled()));
            menuSavitskogoGolayDegreePoligons_3->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay3_9Toogled()));

            QMenu* menuSavitskogoGolayDegreePoligons_4 = new QMenu("Полином 4-й степени", this);
            menuSavitskogoGolayDegreePoligons_4->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay4_7Toogled()));
            menuSavitskogoGolayDegreePoligons_4->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay4_9Toogled()));

            QMenu* menuSavitskogoGolayDegreePoligons_5 = new QMenu("Полином 5-й степени", this);
            menuSavitskogoGolayDegreePoligons_5->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay5_7Toogled()));
            menuSavitskogoGolayDegreePoligons_5->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay5_9Toogled()));

            menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_2);
            menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_3);
            menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_4);
            menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_5);

            menuNoise->addMenu(menuNoiseSavGolay);
            menu->addMenu(menuNoise);
        }
        if (m_attributes->GetAvailablePlugins().contains("SpectralDistance"))
        {
            menu->addAction("Сравнить со спектральными кривыми гиперкуба", this, SLOT(PrepareToCompare()));
        }
        QMenu* menuCompareSpectrInExternalLib = new QMenu("Поиск схожих в спектральной библиотеке", this);
        menuCompareSpectrInExternalLib->addAction("Евклидово расстояние", this, SLOT(ActionEvclideDistToogled()));
        menuCompareSpectrInExternalLib->addAction("Спектральное расстояние", this, SLOT(ActionSpectraDistanceToogled()));
        menuCompareSpectrInExternalLib->addAction("Спектральный угол", this, SLOT(ActionSpectraAngleToogled()));
        menuCompareSpectrInExternalLib->addAction("Спектральная корреляция", this, SLOT(ActionSpectraCorrelationToogled()));
        menuCompareSpectrInExternalLib->addAction("Спектральная информационная дивергенция", this, SLOT(ActionSIDToogled()));
        menuCompareSpectrInExternalLib->addAction("Энтропийная мера близости", this, SLOT(ActionEntropyToogled()));
        menu->addMenu(menuCompareSpectrInExternalLib);
    }
    menu->popup(m_customPlot->mapToGlobal(pos));
}

void PlotterWindow::ActionEvclideDistToogled()
{
    m_modeCompare = SpectrumComparatorFromExternalLibraries::EvclidDist;
    findInSpectralLib();
}
void PlotterWindow::ActionSpectraAngleToogled()
{
    m_modeCompare = SpectrumComparatorFromExternalLibraries::SpectrumAngle;
    findInSpectralLib();
}
void PlotterWindow::ActionSpectraDistanceToogled()
{
    m_modeCompare = SpectrumComparatorFromExternalLibraries::SpectrumDist;
    findInSpectralLib();
}
void PlotterWindow::ActionSpectraCorrelationToogled()
{
    m_modeCompare = SpectrumComparatorFromExternalLibraries::SpectralCorrelation;
    findInSpectralLib();
}
void PlotterWindow::ActionSIDToogled()
{
    m_modeCompare = SpectrumComparatorFromExternalLibraries::SID;
    findInSpectralLib();
}
void PlotterWindow::ActionEntropyToogled()
{
    m_modeCompare = SpectrumComparatorFromExternalLibraries::Entropy;
    findInSpectralLib();
}

void PlotterWindow::findInSpectralLib()
{
    if (m_attributes->GetSpectralLibPath().size() == 0)
    {
        m_attributes->SetSpectralLibPath(QString("D:/"));
    }
    QDir tempDir;
    tempDir.setPath(m_attributes->GetSpectralLibPath());
    tempDir = QFileDialog::getExistingDirectory(this, tr("Поддиректория для спектральных библиотек"), m_attributes->GetSpectralLibPath(), QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);
    if (tempDir.absolutePath().size() != 0)
    {
        QMap<double, Spectr*> spectrMap;
        SpectrumComparatorFromExternalLibraries comparator;
        m_selectedSpectr->SetCurrentDataType(Spectr::NORMED_INTERPOLATE);
        ActionNormalization();
        comparator.GetSimilarSpectra(m_cube, m_selectedSpectr, 0, m_cube->GetCountofChannels() - 1, tempDir.absolutePath(), 15, m_modeCompare, spectrMap);
        QList<Spectr*> spectrList = spectrMap.values();
        for (int i = 0; i < spectrList.size(); i++)
        {
             plotSpectr(spectrList.at(i)->GetCurrentDataX(), spectrList.at(i)->GetCurrentDataY(), spectrList.at(i)->GetTitle());
             m_listSpectr.push_back(spectrList.at(i));
        }
        spectrMap.clear(); spectrList.clear();
    }
}

void PlotterWindow::PrepareToCompare()
{
    leftLine->point1->setCoords(m_cube->GetListOfChannels().at(0), 0);
    leftLine->point2->setCoords(m_cube->GetListOfChannels().at(0),1);

    rightLine->point1->setCoords(m_cube->GetListOfChannels().last(),0);
    rightLine->point2->setCoords(m_cube->GetListOfChannels().last(),1);

    leftLine->setVisible(true);
    rightLine->setVisible(true);

    ui->horizontalSlider_start->setEnabled(true);
    ui->horizontalSlider_end->setEnabled(true);
    ui->horizontalSlider_start->setMaximum(m_cube->GetCountofChannels()-1);
    ui->horizontalSlider_end->setMaximum(m_cube->GetCountofChannels()-1);
    ui->horizontalSlider_start->setValue(0);
    ui->horizontalSlider_end->setValue(m_cube->GetCountofChannels()-1);
    ui->pushButton->setEnabled(true);
    m_customPlot->replot();

}

void PlotterWindow::CompareSpectrWithCube()
{
    ui->horizontalSlider_start->setEnabled(false);
    ui->horizontalSlider_end->setEnabled(false);
    ui->pushButton->setEnabled(false);
    leftLine->setVisible(false);
    rightLine->setVisible(false);
    leftLine->point1->setCoords(m_cube->GetListOfChannels().at(0), 0);
    leftLine->point2->setCoords(m_cube->GetListOfChannels().at(0),1);

    rightLine->point1->setCoords(m_cube->GetListOfChannels().last(),0);
    rightLine->point2->setCoords(m_cube->GetListOfChannels().last(),1);

    m_selectedSpectr->SetCurrentDataType(Spectr::INTERPOLATE_NORMED);
    m_attributes->SetCurrentSpectr(m_selectedSpectr);
    m_attributes->SetStartRangeWave(ui->horizontalSlider_start->value());
    m_attributes->SetEndRangeWave(ui->horizontalSlider_end->value());
    m_attributes->GetAvailablePlugins().value("SpectralDistance")->Execute(m_cube, m_attributes);
}

void PlotterWindow::mouseMoveRequest(QMouseEvent *e)
{
    double x = m_customPlot->xAxis->pixelToCoord(e->x());
    double y = m_customPlot->yAxis->pixelToCoord(e->y());
    if(m_valuesFlag)
    {
        textValues->position->setCoords(x, m_customPlot->yAxis->pixelToCoord(e->y() - 10));
        textValues->setText(QString("x:%1   y:%2").arg(x).arg(y));
        textValues->setFont(QFont(font().family(), 10,QFont::Bold));
        horizLine->point1->setCoords(x,y);
        horizLine->point2->setCoords(0,y);
        vertLine->point1->setCoords(x,y);
        vertLine->point2->setCoords(x,0);
        vertLine->setSelectable(false);
        horizLine->setSelectable(false);
        m_customPlot->replot();
    }
}

void PlotterWindow::removeSelectedGraph()
{
    // удаляем из списка
    for (int i = 0; i < m_listSpectr.size(); i++)
    {
        if (m_listSpectr.at(i) == m_selectedSpectr)
        {
            m_listSpectr.removeAt(i);
            break;
        }
    }
    if (m_customPlot->selectedGraphs().size() > 0)
    {
        m_customPlot->removeGraph(m_customPlot->selectedGraphs().first());
        m_customPlot->replot();
    }
}

void PlotterWindow::removeAllExceptSelectedGraph()
{
    // удаляем из списка
    for (int i = 0; i < m_listSpectr.size(); i++)
    {
        if (m_listSpectr.at(i) != m_selectedSpectr)
        {
            m_listSpectr.removeAt(i);
            break;
        }
    }
    if (m_customPlot->selectedGraphs().size() > 0)
    {
        for (int i = m_customPlot->graphCount()-1; i >= 0; --i)
        {
            if(!m_customPlot->graph(i)->selected())
            {
                m_customPlot->removeGraph(i);
            }
        }
        m_customPlot->replot();
    }
}

void PlotterWindow::onActionValues(bool flag)
{
   m_valuesFlag = flag;
   if (flag)
   {
       textValues->setVisible(true);
       vertLine->setVisible(true);
       horizLine->setVisible(true);
   }
   else
   {
       textValues->setVisible(false);
       vertLine->setVisible(false);
       horizLine->setVisible(false);
   }
   m_customPlot->replot();
}

void PlotterWindow::onActionPoints(bool flag)
{
    m_pointsFlag = flag;
    if(flag)
    {
        for(int i = 0; i < m_customPlot->graphCount(); i++)
            m_customPlot->graph(i)->setScatterStyle(QCPScatterStyle::ssDisc);
    }
    else
    {
        for(int i = 0; i < m_customPlot->graphCount(); i++)
            m_customPlot->graph(i)->setScatterStyle(QCPScatterStyle::ssNone);
    }
    m_customPlot->replot();
}

void PlotterWindow::onActionInterplol()
{
    QString name = "Интерполяция: " + m_selectedSpectr->GetTitle();
    removeSelectedGraph();
    if (m_selectedSpectr->GetCurrentDataType() == Spectr::NONPROCESSING)
    {
       m_selectedSpectr->SetCurrentDataType(Spectr::INTERPOLATE);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::NORMED)
    {
       m_selectedSpectr->SetCurrentDataType(Spectr::NORMED_INTERPOLATE);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::INTERPOLATE)
    {
        m_selectedSpectr->SetCurrentDataType(Spectr::INTERPOLATE);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::INTERPOLATE_NORMED)
    {
        m_selectedSpectr->SetCurrentDataType(Spectr::NORMED_INTERPOLATE);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::NORMED_INTERPOLATE)
    {
        m_selectedSpectr->SetCurrentDataType(Spectr::NORMED_INTERPOLATE);
    }
    m_listSpectr.push_back(m_selectedSpectr);

    plotSpectr(m_selectedSpectr->GetCurrentDataX(), m_selectedSpectr->GetCurrentDataY(), name);
}

void PlotterWindow::ActionNormalization()
{
    QString name = "Нормировка: " + m_selectedSpectr->GetTitle();
    removeSelectedGraph();
    if (m_selectedSpectr->GetCurrentDataType() == Spectr::NONPROCESSING)
    {
       m_selectedSpectr->SetCurrentDataType(Spectr::NORMED);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::NORMED)
    {
       m_selectedSpectr->SetCurrentDataType(Spectr::NORMED);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::INTERPOLATE)
    {
        m_selectedSpectr->SetCurrentDataType(Spectr::INTERPOLATE_NORMED);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::INTERPOLATE_NORMED)
    {
        m_selectedSpectr->SetCurrentDataType(Spectr::INTERPOLATE_NORMED);
    } else if (m_selectedSpectr->GetCurrentDataType() == Spectr::NORMED_INTERPOLATE)
    {
        m_selectedSpectr->SetCurrentDataType(Spectr::NORMED_INTERPOLATE);
    }
    m_selectedSpectr->SetMeasurement(Relative_units);
    m_listSpectr.push_back(m_selectedSpectr);
    plotSpectr(m_selectedSpectr->GetCurrentDataX(), m_selectedSpectr->GetCurrentDataY(), name);
}

void PlotterWindow::ActionNoise3MedianToggled()
{
    m_attributes->SetMaskPixelsCount(3);
    NoiseMedianAlgExecute();
}

void PlotterWindow::ActionNoise5MedianToggled()
{
    m_attributes->SetMaskPixelsCount(5);
    NoiseMedianAlgExecute();
}

void PlotterWindow::ActionNoise7MedianToggled()
{
    m_attributes->SetMaskPixelsCount(7);
    NoiseMedianAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay2_5Toogled()
{
    m_attributes->SetDegreePolinom(2);
    m_attributes->SetMaskPixelsCount(5);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay2_7Toogled()
{
    m_attributes->SetDegreePolinom(2);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay2_9Toogled()
{
    m_attributes->SetDegreePolinom(2);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay3_5Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(5);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay3_7Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay3_9Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay4_7Toogled()
{
    m_attributes->SetDegreePolinom(4);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay4_9Toogled()
{
    m_attributes->SetDegreePolinom(4);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay5_7Toogled()
{
    m_attributes->SetDegreePolinom(5);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay5_9Toogled()
{
    m_attributes->SetDegreePolinom(5);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void PlotterWindow::plotSpectr(QVector<double>& dataX, QVector<double>& dataY, QString& graphName)
{
    if (!m_listSpectr.contains(m_attributes->GetCurrentSpectr()))
    {
        m_listSpectr.push_back(m_attributes->GetCurrentSpectr());
    }
    QVector<double> sortedYArr;
    sortedYArr = dataY;
    qSort(sortedYArr);
    minY = sortedYArr.first();
    maxY = sortedYArr.last();

    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectLegend);
    m_customPlot->legend->setVisible(true);
    /* if (!m_hold)
            m_customPlot->clearGraphs();*/

    m_customPlot->addGraph();
    if(m_pointsFlag)
    {
        m_customPlot->graph()->setScatterStyle( QCPScatterStyle::ssDisc );
    }
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
    if (graphName == "")
    {
        graphName = m_attributes->GetCurrentSpectr()->GetTitle();
    }
    m_customPlot->graph()->setName(graphName);
    m_customPlot->graph()->setData(dataX, dataY);
    m_customPlot->xAxis->setRange(dataX.first(), dataX.last());
    m_customPlot->yAxis->setRange(minY,maxY);
    autoTickCountX = m_customPlot->xAxis->autoTickCount();
    autoTickCountY = m_customPlot->yAxis->autoTickCount();
    m_customPlot->xAxis->setLabel("Длина волны, нм");
    Measurements measurement;
    m_attributes->GetCurrentSpectr()->GetMeasurements(measurement);
    m_customPlot->yAxis->setLabel(GetTextMeasurements(measurement));
    m_customPlot->replot();
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

void PlotterWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        m_customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    if(event->key() == Qt::Key_Control)
        m_customPlot->axisRect()->setRangeZoom(Qt::Vertical);
}

void PlotterWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}

void PlotterWindow::on_actionHold_toggled(bool value)
{
    m_hold = value;
}

void PlotterWindow::on_actionSave_toggled()
{
    m_attributes->SetModeLib(0);
    m_attributes->SetCurrentSpectr(m_selectedSpectr);
    m_attributes->GetAvailablePlugins().value("SpectralLib UI")->Execute(m_cube, m_attributes);
}

void PlotterWindow::NoiseGolayAlgExecute()
{
    bool oldHold = m_hold;
    m_hold = true;
    m_attributes->SetNoiseAlg(Savitski_Golay1D);
    m_attributes->SetApplyToAllCube(false);
    m_attributes->SetCurrentSpectr(m_selectedSpectr);
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_cube, m_attributes);
    m_hold = oldHold;
}



void PlotterWindow::on_horizontalSlider_end_valueChanged(int value)
{
    if (value <= ui->horizontalSlider_start->value())
    {
        ui->horizontalSlider_end->setValue(ui->horizontalSlider_start->value()+1);
    }
}

void PlotterWindow::on_horizontalSlider_start_valueChanged(int value)
{
    if (value >= ui->horizontalSlider_end->value())
    {
        ui->horizontalSlider_start->setValue(ui->horizontalSlider_end->value()-1);
    }
}

void PlotterWindow::on_horizontalSlider_start_sliderMoved(int position)
{
    leftLine->point1->setCoords(m_cube->GetListOfChannels().at(position),0);
    leftLine->point2->setCoords(m_cube->GetListOfChannels().at(position),m_customPlot->yAxis->range().upper);\
    m_customPlot->replot();
}

void PlotterWindow::on_horizontalSlider_end_sliderMoved(int position)
{
    rightLine->point1->setCoords(m_cube->GetListOfChannels().at(position),0);
    rightLine->point2->setCoords(m_cube->GetListOfChannels().at(position),m_customPlot->yAxis->range().upper);\
    m_customPlot->replot();
}
