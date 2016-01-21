#include "histplotterwidget.h"
#include "ui_histplotterwidget.h"




HistPlotterWidget::HistPlotterWidget(HyperCube *cube, Attributes *attr, QWidget *parent) :
    QDialog(parent),  m_cube(cube), BarsAfter(0),
    m_attributes(attr),
    ui(new Ui::HistPlotterWidget)
{
    ui->setupUi(this);

    data = new double[m_cube->GetLines()*m_cube->GetColumns()*sizeof(double)];

    leftLine = new QCPItemStraightLine(ui->beforeCustomPlot); // Удаляется при удалении ui->beforeCustomPlot
    ui->beforeCustomPlot->addItem(leftLine);

    rigthLine = new QCPItemStraightLine(ui->beforeCustomPlot);// Удаляется при удалении ui->beforeCustomPlot
    ui->beforeCustomPlot->addItem(rigthLine);


    BarsBefore = new QCPBars( ui->beforeCustomPlot->xAxis,  ui->beforeCustomPlot->yAxis);
    ui->beforeCustomPlot->addPlottable(BarsBefore);
    ui->beforeCustomPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);

    BarsAfter = new QCPBars( ui->afterCustomPlot->xAxis,  ui->afterCustomPlot->yAxis);
    ui->afterCustomPlot->addPlottable(BarsAfter);
    ui->afterCustomPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);

    PlotDefault();

    ui->sumBoundaryChBox->setChecked(true);
    this->sumBoundary();




}

HistPlotterWidget::~HistPlotterWidget()
{
    emit Close(this);
    delete[] data;
    delete ui;
}

void HistPlotterWidget::accept()
{
    memcpy(m_attributes->GetTempChanel(),data,m_cube->GetLines()*m_cube->GetColumns()*sizeof(double));
    QDialog::accept();
}



void HistPlotterWidget::PlotDefault()
{

    QVector<double> key,value;

    memcpy(data,m_attributes->GetTempChanel(),m_cube->GetLines()*m_cube->GetColumns()*sizeof(double));
    double maxValue = *std::max_element(data,data+m_cube->GetSizeChannel());
    double minValue = *std::min_element(data,data+m_cube->GetSizeChannel());

    double leftBoundary = minValue;
    double rightBoundary = maxValue;

    quint32 HistCount = 100;

    if ((maxValue-minValue) < HistCount) {
        HistCount = maxValue-minValue;
    }

    key.resize(HistCount+1);
    value.resize(HistCount+1);


    double step = (double)(maxValue-minValue)/(HistCount);



    for (unsigned int i = 0; i < m_cube->GetLines(); i++) {
        for (unsigned int j = 0; j < m_cube->GetColumns(); j++) {
        if (data[i*m_cube->GetColumns()+j] < leftBoundary) data[i*m_cube->GetColumns()+j]=leftBoundary;
        if (data[i*m_cube->GetColumns()+j] > rightBoundary) data[i*m_cube->GetColumns()+j]=rightBoundary;
            value[(data[i*m_cube->GetColumns()+j]-minValue)/step]++;
        }
    }




    for (unsigned int k =0; k<HistCount+1; k++) {
        key[k]=minValue+k*step;
    }


    BarsBefore->setData(key,value);



    ui->beforeCustomPlot->rescaleAxes();
    ui->beforeCustomPlot->yAxis->setTickLabels(false);
    ui->beforeCustomPlot->replot();


    ui->leftSlider->setMinimum(leftBoundary);
    ui->rightSlider->setMinimum(leftBoundary);

    ui->leftSlider->setMaximum(rightBoundary);
    ui->rightSlider->setMaximum(rightBoundary);
    ui->rightSlider->setValue(rightBoundary);



    leftLine->point1->setCoords(leftBoundary,0);
    leftLine->point2->setCoords(leftBoundary,1);

    rigthLine->point1->setCoords(rightBoundary,0);
    rigthLine->point2->setCoords(rightBoundary,1);


}

void HistPlotterWidget::on_leftSlider_sliderMoved(int position)
{
    leftLine->point1->setCoords(position,0);
    leftLine->point2->setCoords(position,ui->beforeCustomPlot->yAxis->range().upper);\
    ui->beforeCustomPlot->replot();
}



void HistPlotterWidget::on_rightSlider_sliderMoved(int position)
{
    rigthLine->point1->setCoords(position,0);
    rigthLine->point2->setCoords(position,ui->beforeCustomPlot->yAxis->range().upper);
    ui->beforeCustomPlot->replot();
}




void HistPlotterWidget::on_leftSlider_valueChanged(int value)
{
    if (value > ui->rightSlider->value()-4) ui->leftSlider->setValue(ui->rightSlider->value()-5);
    if (ui->sumBoundaryChBox->isChecked()) sumBoundary();
    else if (ui->gaussChBox->isChecked())  Gauss();

}

void HistPlotterWidget::on_rightSlider_valueChanged(int value)
{
    if (value < ui->leftSlider->value()+4) ui->rightSlider->setValue(ui->leftSlider->value()+5);
    if (ui->sumBoundaryChBox->isChecked()) sumBoundary();
    else if (ui->gaussChBox->isChecked())  Gauss();

}

void HistPlotterWidget::sumBoundary()
{
    if (BarsAfter) {
        BarsAfter->data()->clear();

    qint32 leftBoundary = leftLine->point1->coords().x();
    qint32 rightBoundary = rigthLine->point1->coords().x();

    QVector<double> key,value;



    memcpy(data,m_attributes->GetTempChanel(),m_cube->GetLines()*m_cube->GetColumns()*sizeof(double));


    double maxValue = *std::max_element(data,data+m_cube->GetSizeChannel());
    double minValue = *std::min_element(data,data+m_cube->GetSizeChannel());

    quint32 HistCount = 100;

    if ((maxValue-minValue) < HistCount) {
        HistCount = maxValue-minValue;
    }

    key.resize(HistCount+1);
    value.resize(HistCount+1);


    double step = (double)(maxValue-minValue)/(HistCount);



    for (unsigned int i = 0; i < m_cube->GetLines(); i++) {
        for (unsigned int j = 0; j < m_cube->GetColumns(); j++) {
        if (data[i*m_cube->GetColumns()+j] < leftBoundary) data[i*m_cube->GetColumns()+j]=leftBoundary;
        if (data[i*m_cube->GetColumns()+j] > rightBoundary) data[i*m_cube->GetColumns()+j]=rightBoundary;
            value[(data[i*m_cube->GetColumns()+j]-minValue)/step]++;
        }
    }


    for (int k =0; k<HistCount+1; k++) {
        key[k]=minValue+k*step;
    }


    BarsAfter->setData(key,value);
    ui->afterCustomPlot->rescaleAxes();
    ui->afterCustomPlot->xAxis->setRange(leftBoundary,rightBoundary);
    ui->afterCustomPlot->yAxis->setTickLabels(false);
    ui->afterCustomPlot->replot();
    }
}


void HistPlotterWidget::Gauss() {
    if (BarsAfter) {
        BarsAfter->data()->clear();


    qint32 leftBoundary = leftLine->point1->coords().x();
    qint32 rightBoundary = rigthLine->point1->coords().x();


    QVector<double> key,value;



    memcpy(data,m_attributes->GetTempChanel(),m_cube->GetLines()*m_cube->GetColumns()*sizeof(double));


    double maxValue = rightBoundary;
    double minValue = leftBoundary;



    double sigma = 0;
    double  mu = 0;




    mu = minValue+(maxValue-minValue)/2;
    sigma = (maxValue-minValue)/3;

    double *temp_data = new double[m_cube->GetSizeChannel()];


    for (unsigned int j = 0; j <m_cube->GetSizeChannel(); j++ ) {
        temp_data[j] = data[j] * (1.0/(sigma*sqrt(2*M_PI)))*exp(-(double)pow(data[j]-mu,2)/(2*pow(sigma,2)));
    };


    double maxVal = *std::max_element(temp_data,temp_data+m_cube->GetSizeChannel());
    double minVal = *std::min_element(temp_data,temp_data+m_cube->GetSizeChannel());

    for (unsigned int i = 0; i < m_cube->GetLines(); i++) {
        for (unsigned int j = 0; j < m_cube->GetColumns(); j++) {
            data[i*m_cube->GetColumns()+j] = (maxValue - minValue ) * (temp_data[i*m_cube->GetColumns()+j] - minVal) / (maxVal-minVal)+minValue;
        }
    }


    qint32 HistCount=100;

    if (maxValue-minValue < HistCount) {
        HistCount = maxValue-minValue;
    }


    key.resize(HistCount+1);
    value.resize(HistCount+1);


    double step = (double)(maxValue-minValue)/HistCount;

    for (unsigned int j = 0; j <m_cube->GetSizeChannel(); j++ ) {
        value[(data[j]-minValue)/step]++;
    };


    for (int k =0; k<HistCount+1; k++) {
        key[k]=minValue+k*step;
    }

    delete[] temp_data;



    BarsAfter->setData(key,value);
    ui->afterCustomPlot->rescaleAxes();
   ui->afterCustomPlot->xAxis->setRange(leftBoundary, rightBoundary);
    ui->afterCustomPlot->yAxis->setTickLabels(false);
    ui->afterCustomPlot->replot();
    }
}


void HistPlotterWidget::on_gaussChBox_clicked(bool checked)
{
    if (checked) Gauss();
}

void HistPlotterWidget::on_sumBoundaryChBox_clicked(bool checked)
{
    if (checked) sumBoundary();
}
