#include "rgbimageplugin.h"

#include <QDebug>
#include "rgbimagewindow.h"
#include "settingsdialog.h"

RgbImagePlugin::RgbImagePlugin()
{

}



void RgbImagePlugin::Execute(HyperCube *cube, Attributes *attr)
{

    SettingsDialog dialog(cube) ;
    if (dialog.exec()) {

        RgbImageWindow *rgbImageWindow = new RgbImageWindow();

        rgbImageWindow->plotImage(cube, attr,&dialog);
        rgbImageWindow->show();
    }

}
