#include "rgbimageplugin.h"

#include <QDebug>
#include "rgbimageform.h"

RgbImagePlugin::RgbImagePlugin()
{

}



void RgbImagePlugin::Execute(HyperCube *cube, Attributes *attr)
{

    RgbImageForm *rgbImageForm = new RgbImageForm();
    rgbImageForm->plotImage(cube, attr);


}
