#include "SpectrPlugin.h"

#include "PlotterWindow.h"
#include "PluginAttributes/SpectrPluginAttributes.h"

SpectrPlugin::SpectrPlugin()
{

}

SpectrPlugin::~SpectrPlugin()
{

}

void SpectrPlugin::Execute(HyperCube* cube, IAttributes* attr)
{
    PlotterWindow w;
    w.show();
    w.plotSpectr(cube,((SpectrPluginAttributes*)attr)->GetX(),((SpectrPluginAttributes*)attr)->GetY());
}
