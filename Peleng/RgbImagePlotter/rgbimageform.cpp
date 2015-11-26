#include "rgbimageform.h"
#include "ui_rgbimageform.h"

#include "templeteconvolute.h"
#include "rgbprofile.h"

RgbImageForm::RgbImageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RgbImageForm)
{
    ui->setupUi(this);

}

RgbImageForm::~RgbImageForm()
{
    delete ui;
}

void RgbImageForm::plotImage(HyperCube *cube, Attributes *attr, SettingsDialog *settings)
{
    /*qint32 width = qApp->desktop()->width();
    qint32 heigth = qApp->desktop()->width();*/

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


    ui->label->setPixmap(QPixmap::fromImage(image));
}
