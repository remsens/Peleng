#include "rgbimageplugin.h"

#include <QDebug>
#include "rgbimagewindow.h"
#include "settingsdialog.h"

RgbImagePlugin::RgbImagePlugin()
{

}

RgbImagePlugin::~RgbImagePlugin()
{

}
QObject* RgbImagePlugin::GetObjectPointer()
{
    return this;
}

void RgbImagePlugin::Execute(HyperCube *cube, Attributes *attr)
{
    SettingsDialog dialog(cube);
    if (dialog.exec()) {


        RgbImageWindow *rgbImageWindow = new RgbImageWindow(cube);

        rgbImageWindow->plotImage(attr,&dialog);
        rgbImageWindow->show();
    }

}
