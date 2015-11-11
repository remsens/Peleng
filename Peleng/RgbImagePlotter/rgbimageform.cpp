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

void RgbImageForm::plotImage(HyperCube *cube, Attributes *attr)
{
    RgbProfile profile;
    profile.readFromFile("F:/Peleng/Debug/MainApp/debug/lib/cie.dat");
    //convolute<qint16>(cube);
}
