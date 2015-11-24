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

void Preview2D::Plot(const double* data, const int rows, const int cols, const int numberOfActiveChannel)
{
    setFixedHeight(rows);
    setFixedWidth(cols);
    m_ui->widget2D->resize(rows, cols);
    setWindowTitle(QString("Предпросмотр изображения канала: %1 канал").arg(numberOfActiveChannel));
    int minCMap =  32767;
    int maxCMap = -32767;
    double* dataTemp = new double[rows*cols];
    memcpy(dataTemp, data, rows*cols);
    qsort(dataTemp, rows*cols, sizeof(double), Compare::CompareVariables<double>);
    QCPColorMap* colorMap = new QCPColorMap(m_ui->widget2D->xAxis, m_ui->widget2D->yAxis);
    //colorMap->data()->setSize(rows, cols);
    colorMap->data()->setRange(QCPRange(0, rows-1), QCPRange(0, cols-1));
    m_ui->widget2D->addPlottable(colorMap);
    for (u::uint32 x = 0; x < rows; x++) {
        for (u::uint32 y = 0; y < cols; y++) {
            colorMap->data()->setCell(x, y, data[x * cols + y] );
        }
    }
    minCMap = dataTemp[int(rows*cols*0.02)];
    maxCMap = dataTemp[int(rows*cols*0.98)];
    m_ui->widget2D->rescaleAxes();
    colorMap->setDataRange(QCPRange(minCMap,maxCMap));
    colorMap->setGradient(QCPColorGradient::gpGrayscale);
    colorMap->setInterpolate(false);
    m_ui->widget2D->setInteraction(QCP::iRangeZoom,true);
    m_ui->widget2D->setInteraction(QCP::iRangeDrag,true);
    m_ui->widget2D->replot();
    this->show();
    delete [] dataTemp;

}
