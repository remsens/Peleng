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
    m_actionSave = 0;
    if (m_attributes->GetAvailablePlugins().contains("SpectralLib UI"))
    {
        m_actionSave = new QAction("Сохранить в библиотеку", this);
        ui->menuSpectrum->addAction(m_actionSave);
        QObject::connect(m_actionSave, SIGNAL(triggered(bool)), this, SLOT(on_actionSave_toggled()));
    }
    if (m_attributes->GetFormatExternalSpectr() != 0 && m_attributes->GetExternalSpectrFlag())
    {
        ui->menuSpectrum->removeAction(ui->actionHold);
        //ui->actionHold->destroyed();
    }
}

PlotterWindow::~PlotterWindow()
{
    delete m_actionSave;
    delete ui;
}

void PlotterWindow::closeEvent(QCloseEvent *) {
    emit closePlotterWindow(this);
}

void PlotterWindow::plotSpectr(uint dataX, uint dataY)
{
    QString err;
    try
    {    //если можем получить точку гиперкуба
        QVector<double> xArr, yArr;
        if (m_attributes->GetExternalSpectrFlag())
        {
            xArr = m_attributes->GetXUnits();
            yArr = m_attributes->GetYUnits();
        } else
        {
            quint16 Chnls = m_cube->GetCountofChannels();
            qint16* pSpectrValues = new qint16[Chnls];
            m_cube->GetSpectrumPoint(dataX, dataY,pSpectrValues); // записали в pSpectrValues из гиперкуба
            QList<double> Wawes;
            Wawes.append(m_cube->GetListOfChannels());
            for (quint16 i = 0; i < Chnls; ++i )
            {
               xArr.push_back(Wawes[i]);
               yArr.push_back(pSpectrValues[i]);
            }
            delete [] pSpectrValues;
        }
        QVector<double> sortedYArr;
        sortedYArr = yArr;
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
    m_attributes->GetAvailablePlugins().value("SpectralLib UI")->Execute(m_cube, m_attributes);
}

