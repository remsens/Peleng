#include "Preview2D.h"
#include "ui_Preview2D.h"

#include "../Library/Utils/Compare.h"
#include "../Library/Types.h"

Preview2D::Preview2D(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Preview2D)
{
    m_ui->setupUi(this);
    //TODO
    //setWindowIcon(QIcon(":/logo/IconsPlotter/PlotterLogo.ico"));
    setAttribute(Qt::WA_DeleteOnClose, true);
}

Preview2D::~Preview2D()
{
    delete m_ui;
}

void Preview2D::Plot(double* data, const int rows, const int cols, const int numberOfActiveChannel)
{
    if(rows>cols)
         this->resize(this->width(), this->width() * cols / rows);
    else
        this->resize(this->width() * rows / cols, this->width() );
    setWindowTitle(QString("Предпросмотр изображения канала: %1 канал").arg(numberOfActiveChannel));
    int minCMap =  32767;
    int maxCMap = -32767;
//    double* dataTemp = new double[size];
//    memcpy(dataTemp, data, size);
//    qsort(dataTemp, size, sizeof(double), Compare::CompareVariables<double>);
    QCPColorMap* colorMap = new QCPColorMap(m_ui->widget2D->xAxis, m_ui->widget2D->yAxis);
    colorMap->setKeyAxis(m_ui->widget2D->xAxis);
    colorMap->setValueAxis(m_ui->widget2D->yAxis);
    colorMap->data()->setSize(rows, cols);
    colorMap->data()->setRange(QCPRange(0, rows-1), QCPRange(0, cols-1));
    m_ui->widget2D->addPlottable(colorMap);
    for (u::uint32 x = 0; x < rows; x++) {
        for (u::uint32 y = 0; y < cols; y++) {
            colorMap->data()->setCell(x, y, data[x * cols + y] );
        }
    }
    qsort(data, rows*cols, sizeof(double), Compare::CompareVariables<double>);
    minCMap = data[int(rows*cols*0.02)];
    maxCMap = data[int(rows*cols*0.98)];
    m_ui->widget2D->rescaleAxes();
    colorMap->rescaleDataRange(true);
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->setInterpolate(false);
    m_ui->widget2D->setInteraction(QCP::iRangeZoom,true);
    m_ui->widget2D->setInteraction(QCP::iRangeDrag,true);
    m_ui->widget2D->replot();
    this->show();
    //delete [] dataTemp;

}
