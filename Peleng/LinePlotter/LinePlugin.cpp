#include "LinePlugin.h"
#include "LinePlotterWindow.h"

LinePlugin::LinePlugin(QObject* parent)
    : QObject(parent)
{

}

LinePlugin::~LinePlugin()
{

}

void LinePlugin::Execute(HyperCube* cube, Attributes* attr)
{    
    LinePlotterWindow* linePlotter = new LinePlotterWindow(attr);

    if (attr->GetPointsList().size() >= 2)
    {
        linePlotter->plotSpectrLine(cube, attr->GetPointsList().at(0).x, attr->GetPointsList().at(1).x,
                                          attr->GetPointsList().at(0).y, attr->GetPointsList().at(1).y,
                                          attr->GetPointsList().at(0).z, attr->GetPointsList().at(1).z);
    }
    linePlotter->activateWindow();
    linePlotter->show();
}

