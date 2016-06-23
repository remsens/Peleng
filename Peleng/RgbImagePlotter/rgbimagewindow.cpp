#include "rgbimagewindow.h"
#include "ui_rgbimagewindow.h"


#include "templeteconvolute.h"
#include "rgbprofile.h"

RgbImageWindow::RgbImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RgbImageWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/rgb/icons/icons/RGB.png"));

    initCustomplotSettings();

//    ui->toolBar->addAction(QIcon(":/rgb/icons/icons/zoom_in.png"),"Zoom In",ui->widget,SLOT(ZoomInSetter()));
//    ui->toolBar->addAction(QIcon(":/rgb/icons/icons/zoom_out.png"),"Zoom Out",ui->widget,SLOT(ZoomOutSetter()));
//    ui->toolBar->addAction(QIcon(":/rgb/icons/icons/move.png"),"Zoom Scroll",ui->widget,SLOT(MoveSetter()));
//    QObject::connect(ui->widget,SIGNAL(updateSize()),this,SLOT(updateSize()));

}

RgbImageWindow::~RgbImageWindow()
{
    delete ui;
}

void RgbImageWindow::plotImage(HyperCube *cube, Attributes *attr, SettingsDialog *settings)
{
    m_cube = cube;
    QImage image(cube->GetLines(), cube->GetColumns(), QImage::Format_RGB32);
    setAspectRatio();

    if (settings->getTypeOfMethod()!=2) {
        RgbProfile profile;
        if (settings->getTypeOfMethod()==0) profile.readStd((RgbProfile::CieProfiles)settings->getTypeOfStdProfile());
        else if (settings->getTypeOfMethod()==1) profile.readFromFile(settings->getFileName());
        switch (cube->GetFormatType()) {
            case type_int8: RgbProfilePlot<qint8>(cube,profile,image); break;
            case type_uint8: RgbProfilePlot<quint8>(cube,profile,image); break;
            case type_int16: RgbProfilePlot<qint16>(cube,profile,image); break;
            case type_uint16: RgbProfilePlot<quint16>(cube,profile,image); break;
            case type_int32: RgbProfilePlot<qint32>(cube,profile,image); break;
            case type_uint32: RgbProfilePlot<quint32>(cube,profile,image); break;
            case type_int64: RgbProfilePlot<qint64>(cube,profile,image); break;
            case type_uint64: RgbProfilePlot<quint64>(cube,profile,image); break;
            case type_float:RgbProfilePlot<float>(cube,profile,image); break;
            case type_double: RgbProfilePlot<double>(cube,profile,image); break;
            default: RgbProfilePlot<qint8>(cube,profile,image); break;
        }
    }
    else if (settings->getTypeOfMethod()==2) {
        switch (cube->GetFormatType()) {
            case type_int8: RgbChannelPlot<qint8>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_uint8: RgbChannelPlot<quint8>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_int16: RgbChannelPlot<qint16>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_uint16: RgbChannelPlot<quint16>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_int32: RgbChannelPlot<qint32>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_uint32: RgbChannelPlot<quint32>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_int64: RgbChannelPlot<qint64>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_uint64: RgbChannelPlot<quint64>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_float:RgbChannelPlot<float>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            case type_double: RgbChannelPlot<double>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
            default: RgbChannelPlot<qint8>(cube,settings->getRCh(),settings->getGCh(),settings->getBCh(),image); break;
        }
    }
    else {
        return;
    }

    showAtCustomplot(image);

//    ui->centralwidget->adjustSize();
//    this->adjustSize();
}

void RgbImageWindow::showAtCustomplot(QImage image)
{

   QCPItemPixmap *MyImage = new QCPItemPixmap(ui->widget);
   QPixmap pixmap = QPixmap::fromImage(image);
   MyImage->setPixmap(pixmap);
   MyImage->topLeft->setType(QCPItemPosition::ptPlotCoords);
   MyImage->bottomRight->setType(QCPItemPosition::ptPlotCoords);
   MyImage->topLeft->setCoords(ui->widget->xAxis->range().lower, ui->widget->yAxis->range().upper);
   MyImage->bottomRight->setCoords(ui->widget->xAxis->range().upper, ui->widget->yAxis->range().lower);
   MyImage->setScaled(true,Qt::IgnoreAspectRatio);
   ui->widget->addItem(MyImage);

}

void RgbImageWindow::initCustomplotSettings()
{
    ui->widget->setInteraction(QCP::iRangeDrag , true);
    ui->widget->setInteraction(QCP::iRangeZoom  , true);
    ui->widget->yAxis->setTicks(false);
    ui->widget->xAxis->setTicks(false);
    ui->widget->xAxis->setTickLabels(false);
    ui->widget->yAxis->setTickLabels(false);
    ui->widget->xAxis->setVisible(false);
    ui->widget->yAxis->setVisible(false);
    ui->widget->axisRect()->setAutoMargins(QCP::msNone);
    ui->widget->axisRect()->setMargins(QMargins(0,0,0,0));
}

void RgbImageWindow::setAspectRatio()
{
    int rows = m_cube->GetLines();
    int cols = m_cube->GetColumns();
    double RowsToCols = (double)rows / (double)cols;
    const int defaultWidth = 500;
    const int defaultHeight = 500;
    if(rows>cols)
    {
        this->resize(defaultWidth, defaultWidth / RowsToCols);
    }
    else
    {
        this->resize(defaultHeight*RowsToCols, defaultHeight);

    }
}

void RgbImageWindow::updateSize()
{
    ui->centralwidget->adjustSize();
    this->adjustSize();
}

