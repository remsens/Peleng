#include "Preview2D.h"
#include "ui_Preview2D.h"

#include "../Library/Utils/Compare.h"
#include "../Library/Types.h"
#include "../Library/Attributes/Attributes.h"
Preview2D::Preview2D(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Preview2D)
{
    m_ui->setupUi(this);
    m_cPlot = new QCustomPlot(this);
    m_ui->verticalLayout_2->addWidget(m_cPlot);
    colorMap = new QCPColorMap(m_cPlot->xAxis, m_cPlot->yAxis);
    m_cPlot->addPlottable(colorMap);
    connect(m_cPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(mousePressOnColorMap(QMouseEvent*)));
    connect(m_cPlot,SIGNAL(customContextMenuRequested(QPoint)),SLOT(ShowContextMenu(QPoint)));
    //TODO
    //setWindowIcon(QIcon(":/logo/IconsPlotter/PlotterLogo.png"));
    setAttribute(Qt::WA_DeleteOnClose, true);
}

Preview2D::~Preview2D()
{
    delete m_ui;
}

void Preview2D::Plot(double* data, const int rows, const int cols, const QString& title)
{
    if(rows>cols)
         this->resize(this->width(), this->width() * cols / rows);
    else
        this->resize(this->width() * rows / cols, this->width() );
    setWindowTitle(title);
    int minCMap =  32767;
    int maxCMap = -32767;
    colorMap->setKeyAxis(m_cPlot->xAxis);
    colorMap->setValueAxis(m_cPlot->yAxis);
    colorMap->data()->setSize(rows, cols);
    colorMap->data()->setRange(QCPRange(0, rows-1), QCPRange(0, cols-1));
    for (u::int32 x = 0; x < rows; x++) {
        for (u::int32 y = 0; y < cols; y++) {
            colorMap->data()->setCell(x, y, data[x * cols + y] );
        }
    }
    qsort(data, rows*cols, sizeof(double), Compare::CompareVariables<double>);
    minCMap = data[int(rows*cols*0.02)];
    maxCMap = data[int(rows*cols*0.98)];
    m_cPlot->rescaleAxes();
    colorMap->rescaleDataRange(true);
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->setInterpolate(false);
    m_cPlot->setInteraction(QCP::iRangeZoom,true);
    m_cPlot->setInteraction(QCP::iRangeDrag,true);
    m_cPlot->replot();
    this->show();
 }
void Preview2D::plotPointsOn2D(QVector<double> x, QVector<double> y)
{
    m_cPlot->clearGraphs(); //удаляем предыдущий график
    m_cPlot->addGraph();
    m_cPlot->graph()->setData(x, y);
    m_cPlot->graph()->setLineStyle(QCPGraph::lsNone);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::red));
    myScatter.setBrush(Qt::yellow);
    //myScatter.setSize(5);
    m_cPlot->graph()->setScatterStyle(myScatter);
    m_cPlot->replot();
}

void Preview2D::ShowContextMenu(const QPoint pos)
{
    QMenu* contextMenu = new QMenu(this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose, true);
    contextMenu->setStyleSheet("border: 0px solid black;");
    if ( Attributes::I()->GetAvailablePlugins().contains("Spectr UI"))
    {
        contextMenu->addAction(QIcon(":/IconsCube/iconsCube/Plot.png"),"Спектр",this, SLOT(prepareToPlotSpectr()));

    }
    int x = m_cPlot->xAxis->pixelToCoord(pos.x());
    int y = m_cPlot->yAxis->pixelToCoord(pos.y());
    qDebug()<<"ShowContextMenu x = "<<x;
    qDebug()<<"ShowContextMenu y = "<<y;
    contextMenu->popup(m_cPlot->mapToGlobal(pos));
}

void Preview2D::prepareToPlotSpectr()
{

}

void Preview2D::mousePressOnColorMap(QMouseEvent *e)
{
    int x = m_cPlot->xAxis->pixelToCoord(e->pos().x());
    int y = m_cPlot->yAxis->pixelToCoord(e->pos().y());
    qDebug()<<"mousePressOnColorMap x = "<<x;
    qDebug()<<"mousePressOnColorMap y = "<<y;

}
/*
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    evalDataCordsFromMouse(event->x(),event->y());
    //qDebug() <<"round XYZ" <<"x:"<< m_dataX<< " y:"<< m_dataY<< " z:"<< m_dataZ << endl<<endl;
    if ((event->button() == Qt::RightButton) && (contexMenuEnabled==true))
    {
        ShowContextMenu(event->pos());
    }
    emit signalCurrentDataXYZ(m_dataX, m_dataY, m_dataZ); // нужен для LinePlotter'а. отправка сигнала только тогда, когда клик по кубу
}
*/
