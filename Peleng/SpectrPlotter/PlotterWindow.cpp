#include "PlotterWindow.h"
#include "ui_PlotterWindow.h"

#include <QtAlgorithms>
#include "../Library/GenericExc.h"
#include <QDebug>
#include <QMessageBox>

PlotterWindow::PlotterWindow(HyperCube* cube, Attributes* attr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlotterWindow)
    , minY(70000)
    , maxY(-10000)
    , m_cube(cube)
    , m_attributes(attr)

{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowIcon(QIcon(":/logo/IconsPlotter/PlotterLogo.ico"));
    ui->setupUi(this);
    m_customPlot = (QCustomPlot*) ui->PlotWidget;
    m_hold = false;
    initSize = size();

    QPropertyAnimation* panim = new QPropertyAnimation(this, "windowOpacity");
    panim->setDuration(300);
    panim->setStartValue(0);
    panim->setEndValue(1);
    panim->setEasingCurve(QEasingCurve::InCirc);
    panim->start(QAbstractAnimation::DeleteWhenStopped);
    if (m_attributes->GetFormatExternalSpectr() != 0 && m_attributes->GetExternalSpectrFlag())
    {
        ui->menuSpectrum->removeAction(ui->actionHold);
        //ui->actionHold->destroyed();
    }
    if (m_attributes->GetExternalSpectrFlag())
    {
        m_descriptionExternalSpectr.append(m_attributes->GetSpectrumDescription());
    }
    connect(m_customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),  SLOT(graphClicked(QCPAbstractPlottable*)));
    m_customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
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
    m_attributes->SetXUnit(m_xArr);
    m_attributes->SetYUnit(m_yArr);
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_cube, m_attributes);
    m_hold = oldHold;
}

void PlotterWindow::graphClicked(QCPAbstractPlottable * plottable)
{
    QString grafName = plottable->name();
    QList<QCPData> XandYlist = dynamic_cast<QCPGraph*>(plottable)->data()->values();
    m_xArr.clear();
    m_yArr.clear();
    foreach(QCPData xy,XandYlist)
    {
        m_xArr.append(xy.key);
        m_yArr.append(xy.value);
    }
    //    XandY.at(i).key; //х
    //    XandY.at(i).value; //у
}

void PlotterWindow::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    if (m_customPlot->selectedGraphs().size() > 0)
    {
        menu->addAction("Удалить выделенный график", this, SLOT(removeSelectedGraph()));
        menu->addAction("Оставить выделенный график",this,SLOT(removeAllExceptSelectedGraph()));
        if (m_attributes->GetAvailablePlugins().contains("SpectralLib UI"))
        {
            menu->addAction("Сохранить в библиотеку",this,SLOT(on_actionSave_toggled()));
        }
        if (m_attributes->GetAvailablePlugins().contains("Noise Remover"))
        {
            QMenu* menuNoise = new QMenu("Фильтры", this);

            QMenu* menuNoiseMedian = new QMenu("Медианный фильтр", this);
            menuNoiseMedian->addAction("Медианный 3х3", this, SLOT(ActionNoise3MedianToggled()));
            menuNoiseMedian->addAction("Медианный 5х5", this, SLOT(ActionNoise5MedianToggled()));
            menuNoiseMedian->addAction("Медианный 7х7", this, SLOT(ActionNoise7MedianToggled()));
            menuNoise->addMenu(menuNoiseMedian);

            QMenu* menuNoiseSavGolay = new QMenu("Савитского-Голау фильтр", this);

            QMenu* menuSavitskogoGolayDegreePoligons_2_3 = new QMenu("Квадратичный/кубический полином", this);
            menuSavitskogoGolayDegreePoligons_2_3->addAction("Маска: 5 пикселей", this, SLOT(ActionNoiseSavitGolay2_3_5Toogled()));
            menuSavitskogoGolayDegreePoligons_2_3->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay2_3_7Toogled()));
            menuSavitskogoGolayDegreePoligons_2_3->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay2_3_9Toogled()));

            QMenu* menuSavitskogoGolayDegreePoligons_4_5 = new QMenu("Полином четвертой/пятой степени", this);
            menuSavitskogoGolayDegreePoligons_4_5->addAction("Маска: 7 пикселей", this, SLOT(ActionNoiseSavitGolay4_5_7Toogled()));
            menuSavitskogoGolayDegreePoligons_4_5->addAction("Маска: 9 пикселей", this, SLOT(ActionNoiseSavitGolay4_5_9Toogled()));

            menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_2_3);
            menuNoiseSavGolay->addMenu(menuSavitskogoGolayDegreePoligons_4_5);
            menuNoise->addMenu(menuNoiseSavGolay);
            menu->addMenu(menuNoise);
        }

    }
    menu->popup(m_customPlot->mapToGlobal(pos));
}

void PlotterWindow::removeSelectedGraph()
{
    if (m_customPlot->selectedGraphs().size() > 0)
    {
        m_customPlot->removeGraph(m_customPlot->selectedGraphs().first());
        m_customPlot->replot();
    }
}

void PlotterWindow::removeAllExceptSelectedGraph()
{
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

void PlotterWindow::plotSpectr(uint dataX, uint dataY)
{

    initSize = size();

    m_xArr.clear();
    m_yArr.clear();

    QString err;
    try
    {    //если можем получить точку гиперкуба
        m_xArr.clear();
        m_yArr.clear();
        if (m_attributes->GetExternalSpectrFlag())
        {
            m_xArr = m_attributes->GetXUnits();
            m_yArr = m_attributes->GetYUnits();
        } else
        {
            quint16 Chnls = m_cube->GetCountofChannels();
            qint16* pSpectrValues = new qint16[Chnls];
            m_cube->GetSpectrumPoint(dataX, dataY,pSpectrValues); // записали в pSpectrValues из гиперкуба
            QList<double> Wawes;
            Wawes.append(m_cube->GetListOfChannels());
            for (quint16 i = 0; i < Chnls; ++i )
            {
               m_xArr.push_back(Wawes[i]);
               m_yArr.push_back(pSpectrValues[i]);
            }
            delete [] pSpectrValues;
        }
        QVector<double> sortedYArr;
        sortedYArr = m_yArr;
        qSort(sortedYArr);
        if (sortedYArr.first() < minY )
            minY = sortedYArr.first();
        if (sortedYArr.last() > maxY )
            maxY = sortedYArr.last();

        QString grafName;
        if (m_attributes->GetExternalSpectrFlag())
        {
            grafName.append(m_attributes->GetSpectrumDescription().at(0).title).append(m_attributes->GetSpectrumDescription().at(0).description);
        } else
        {
            grafName.append("X:");
            grafName.append(QString::number(dataX));
            grafName.append(" Y:");
            grafName.append(QString::number(dataY));
        }

        m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectLegend);

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
        m_customPlot->graph()->setData(m_xArr, m_yArr);
        m_customPlot->xAxis->setRange(m_xArr.first(),m_xArr.last());
        m_customPlot->yAxis->setRange(minY,maxY);
        if (m_attributes->GetExternalSpectrFlag())
        {
            switch (m_attributes->GetFormatExternalSpectr())
            {
                case 1: {
                    // aster
                    for (int i = 0; i < m_attributes->GetSpectrumDescription().size(); i++)
                    {
                        if (m_attributes->GetSpectrumDescription().at(i).title.compare("X Units:") == 0)
                        {
                            m_customPlot->xAxis->setLabel(m_attributes->GetSpectrumDescription().at(i).description);
                        } else if (m_attributes->GetSpectrumDescription().at(i).title.compare("Y Units:") == 0)
                        {
                            m_customPlot->yAxis->setLabel(m_attributes->GetSpectrumDescription().at(i).description);
                        }
                    }
                    break;
                }
                default: {
                   m_customPlot->xAxis->setLabel("Длина волны, нм");
                   m_customPlot->yAxis->setLabel("Яркость");
                   break;
                }
            }
        } else
        {
            m_customPlot->xAxis->setLabel("Длина волны, нм");
            m_customPlot->yAxis->setLabel("Яркость");
        }
        m_customPlot->replot();
        autoTickCountX = m_customPlot->xAxis->autoTickCount();
        autoTickCountY = m_customPlot->yAxis->autoTickCount();
    } catch(const GenericExc& exc)
    {
        //m_customPlot->replot();
        err = exc.GetWhat();
    } catch(...)
    {
       err = tr("Неизвестная ошибка");
    }
    if (!err.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", err);
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

void PlotterWindow::on_actionSave_toggled()
{
    m_attributes->SetModeLib(0);
    m_attributes->ClearUnitsLists();
    m_attributes->SetXUnit(m_xArr);
    m_attributes->SetYUnit(m_yArr);
    m_attributes->SetDescriptionSpectr(m_descriptionExternalSpectr);
    m_attributes->GetAvailablePlugins().value("SpectralLib UI")->Execute(m_cube, m_attributes);
}

void PlotterWindow::NoiseGolayAlgExecute()
{
    bool oldHold = m_hold;
    m_hold = true;
    m_attributes->SetNoiseAlg(Savitski_Golay1D);
    m_attributes->SetApplyToAllCube(false);
    m_attributes->SetXUnit(m_xArr);
    m_attributes->SetYUnit(m_yArr);
    m_attributes->GetAvailablePlugins().value("Noise Remover")->Execute(m_cube, m_attributes);
    m_hold = oldHold;
}

void PlotterWindow::ActionNoiseSavitGolay2_3_5Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(5);
    NoiseGolayAlgExecute();
}
void PlotterWindow::ActionNoiseSavitGolay2_3_7Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay2_3_9Toogled()
{
    m_attributes->SetDegreePolinom(3);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay4_5_7Toogled()
{
    m_attributes->SetDegreePolinom(4);
    m_attributes->SetMaskPixelsCount(7);
    NoiseGolayAlgExecute();
}

void PlotterWindow::ActionNoiseSavitGolay4_5_9Toogled()
{
    m_attributes->SetDegreePolinom(4);
    m_attributes->SetMaskPixelsCount(9);
    NoiseGolayAlgExecute();
}
