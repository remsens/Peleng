#include "rgbimagewindow.h"
#include "ui_rgbimagewindow.h"


#include "templeteconvolute.h"
#include "rgbprofile.h"

RgbImageWindow::RgbImageWindow(QWidget *parent) :
    QMainWindow(parent), zoomRect(-1,-1,-1,-1),
    ui(new Ui::RgbImageWindow)
{
    ui->setupUi(this);

    // Для zoom


    ui->toolBar->addAction(QIcon(":/rgb/icons/icons/1447945148_12_Zoom_in.ico"),"Zoom In",ui->widget,SLOT(ZoomInSetter()));
    ui->toolBar->addAction(QIcon(":/rgb/icons/icons/1447945175_13_Zoom_out.ico"),"Zoom Out",ui->widget,SLOT(ZoomOutSetter()));
    ui->toolBar->addAction(QIcon(":/rgb/icons/icons/1447945314_move.ico"),"Zoom Scroll",ui->widget,SLOT(MoveSetter()));
    QObject::connect(ui->widget,SIGNAL(updateSize()),this,SLOT(updateSize()));
   /* ui->label->installEventFilter(this);
    ui->label->setFocusPolicy(Qt::StrongFocus);*/
    //drawing =false;
   // this->move(30,30);
}

RgbImageWindow::~RgbImageWindow()
{
    delete ui;
}

void RgbImageWindow::plotImage(HyperCube *cube, Attributes *attr, SettingsDialog *settings)
{

    QImage image(cube->GetLines(), cube->GetColumns(), QImage::Format_RGB32);


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

    mainPixmap = QPixmap::fromImage(image);

    ((ZoomImage*)ui->widget)->setMainPixmap(mainPixmap);
    //updateSize();


  //  ui->label->setPixmap(mainPixmap.scaledToWidth(800));
}




void RgbImageWindow::ZoomIn()
{
    //isZoomIn = true;
  //  ui->label->setCursor(QCursor(Qt::CrossCursor));
}

void RgbImageWindow::ZoomOut()
{
    //isZoomIn = false;
//    ui->label->setCursor(QCursor(Qt::ArrowCursor));
}

bool RgbImageWindow::eventFilter(QObject *obj, QEvent *event)
{
   /* if (obj==ui->label) {


   if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        //zoomRect.setTopLeft(QPoint());
        return true;
    } else if (event->type() == QEvent::MouseTrackingChange) {


    } else if (event->type() == QEvent::Wheel) {
        QWheelEvent *event = static_cast<QWheelEvent *> (event);
        //zoom(1.1);
        //qDebug() << event->angleDelta() / (8 * 15);
    }*/

  /* if (event->type() == QEvent::MouseButtonPress) {
          QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
          zoomRect.setTopLeft(QPoint(mouseEvent->x(),mouseEvent->y()));
          drawing = true;
          return true;
        }

     else if (event->type() == QEvent::MouseButtonRelease) {
        zoomRect.setRect(-1,-1,-1,-1);
        drawing =false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        zoomRect.setBottomRight(QPoint(mouseEvent->x(),mouseEvent->y()));

        //ui->label->repaint();
        return true;
    } else if (event->type() == QEvent::Paint) {

        //if (drawing) return true;

       // painter.drawRect(zoomRect);

        ui->label->setPixmap(mainPixmap.scaledToWidth(800));

        return true;
    }*/
  /*  } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }*/
}

void RgbImageWindow::updateSize()
{

    QRect geom= this->geometry();
    geom.setSize(this->minimumSize());
    this->setGeometry(geom);


    //this->updateGeometry();
    //this->resize(((ZoomImage*)ui->widget)->size());
}

/*void RgbImageWindow::paintEvent(QPaintEvent *event)
{
}*/
