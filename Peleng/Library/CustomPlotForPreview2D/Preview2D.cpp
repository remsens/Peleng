#include "Preview2D.h"
#include "ui_Preview2D.h"

#include "../Library/Utils/Compare.h"
#include "../Library/Types.h"

Preview2D::Preview2D(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Preview2D)
{
    m_ui->setupUi(this);
    m_cPlot = new QCustomPlot(this);
    m_ui->verticalLayout_2->addWidget(m_cPlot);
    colorMap = new QCPColorMap(m_cPlot->xAxis, m_cPlot->yAxis);
    m_cPlot->addPlottable(colorMap);
    m_cPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_cPlot,SIGNAL(mousePress(QMouseEvent*)),SLOT(mousePressOnColorMap(QMouseEvent*)));
    connect(m_cPlot,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowContextMenu(QPoint)));
 	m_cPlot->yAxis->setTicks(false);
    m_cPlot->xAxis->setTicks(false);
    m_cPlot->xAxis->setVisible(false);
    m_cPlot->yAxis->setVisible(false);
    m_cPlot->axisRect()->setAutoMargins(QCP::msNone);
    m_cPlot->axisRect()->setMargins(QMargins(0,0,0,-1));// -1 устраняет баг с полосой белых пикселей при 0
    //TODO
    //setWindowIcon(QIcon(":/logo/IconsPlotter/PlotterLogo.png"));
    setAttribute(Qt::WA_DeleteOnClose, true);
}

Preview2D::~Preview2D()
{
    delete m_ui;
}

void Preview2D::Plot(double* data, const int rows, const int cols, const QString& title, HyperCube *cube, Attributes *attr)
{
    m_rows = rows;
    m_cols = cols;
    m_cube = cube;
    m_attr = attr;
    if(rows>cols)
         this->resize(this->width(), this->width() * cols / rows + m_ui->frameSliders->height());
    else
        this->resize(this->width() * rows / cols, this->width() + m_ui->frameSliders->height() );
//    if(rows>cols)
//         m_cPlot->resize(600, 600 * cols / rows);
//    else
//        m_cPlot->resize(600 * rows / cols, 600 );
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
    minCMap = data[int( (rows*cols-1) * 0.02)];
    maxCMap = data[int( (rows*cols-1) * 0.98)];
    qDebug()<<data[0];
    qDebug()<<minCMap;
    qDebug()<<m_ui->SliderMin->value();
    qDebug()<<m_ui->SliderMax->value();
    m_cPlot->rescaleAxes();
    colorMap->rescaleDataRange(true);
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->setInterpolate(false);
    m_cPlot->setInteraction(QCP::iRangeZoom,true);
    m_cPlot->setInteraction(QCP::iRangeDrag,true);
    m_cPlot->replot();
    this->show();

    //значения для слайдеров
    m_ui->SliderMin->setMinimum(data[0]);
    m_ui->SliderMax->setMinimum(data[0]);
    m_ui->SliderMin->setMaximum(data[rows*cols-1]);
    m_ui->SliderMax->setMaximum(data[rows*cols-1]);
    m_ui->SliderMax->setValue(maxCMap); //сначала инициализируем именно этот слайдер
    m_ui->SliderMin->setValue(minCMap);
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

void Preview2D::ShowContextMenu(QPoint pos)
{
    qDebug()<<"entered ShowContextMenu";
    QMenu* contextMenu = new QMenu(this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose, true);
    contextMenu->setStyleSheet("border: 0px solid black;");
    if(m_cube != nullptr && m_attr != nullptr)
        contextMenu->addAction(QIcon(":/IconsCube/iconsCube/Plot.png"),"Спектр",this, SLOT(prepareToPlotSpectr()));
    int x = m_cPlot->xAxis->pixelToCoord(pos.x());
    int y = m_cPlot->yAxis->pixelToCoord(pos.y());
    qDebug()<<"ShowContextMenu x = "<<x;
    qDebug()<<"ShowContextMenu y = "<<y;
    contextMenu->popup(m_cPlot->mapToGlobal(pos));
}

void Preview2D::prepareToPlotSpectr()
{
    m_attr->ClearList();
    m_attr->SetPoint(m_dataX, m_dataY, 0);
    m_attr->SetExternalSpectrFlag(false);
    m_attr->GetAvailablePlugins().value("Spectr UI")->Execute(m_cube, m_attr);
}

void Preview2D::mousePressOnColorMap(QMouseEvent *e)
{
    int x = m_cPlot->xAxis->pixelToCoord(e->pos().x());
    int y = m_cPlot->yAxis->pixelToCoord(e->pos().y());
    if(x<0)
        m_dataX = 0;
    else if(x>m_rows)
        m_dataX = m_rows-1;
    else
        m_dataX = x;

    if(y<0)
        m_dataY = 0;
    else if(y>m_cols)
        m_dataY = m_cols-1;
    else
        m_dataY = y;

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
void Preview2D::on_SliderMin_valueChanged(int value)
{
    if (value >  m_ui->SliderMax->value())
    {
        m_ui->SliderMin->setValue(m_ui->SliderMax->value() - 1);
        return;
    }
    double max =  colorMap->dataRange().upper;
    colorMap->setDataRange(QCPRange(value,max));
    m_cPlot->replot();
}

void Preview2D::on_SliderMax_valueChanged(int value)
{
    if (value <  m_ui->SliderMin->value())
    {
        m_ui->SliderMax->setValue(m_ui->SliderMin->value() + 1);
        return;
    }
    double min =  colorMap->dataRange().lower;
    colorMap->setDataRange(QCPRange(min,value));
    m_cPlot->replot();
}
