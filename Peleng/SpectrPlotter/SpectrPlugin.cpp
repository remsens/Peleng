#include "SpectrPlugin.h"

#include <QSharedPointer>
#include "PlotterWindow.h"
#include "PluginAttributes/SpectrPluginAttributes.h"

SpectrPlugin::SpectrPlugin(QObject* parent)
{

}

SpectrPlugin::~SpectrPlugin()
{

}

void SpectrPlugin::Execute(HyperCube* cube, IAttributes* attr)
{
    QSharedPointer<PlotterWindow> w(new PlotterWindow);
    w->show();
    w->plotSpectr(cube,((SpectrPluginAttributes*)attr)->GetX(),((SpectrPluginAttributes*)attr)->GetY());
}
