#include "LinePlugin.h"
#include <QSharedPointer>
#include "LinePlotterWindow.h"
#include "../Library/PluginAttributes/LinePluginAttributes.h"

LinePlugin::LinePlugin(QObject* parent)
{

}

LinePlugin::~LinePlugin()
{

}
void LinePlugin::Execute(HyperCube* cube, IAttributes* attr)
{
    QSharedPointer<LinePlotterWindow> w(new LinePlotterWindow);
    w->show();
    //w->plotSpectrLine(cube,((SpectrPluginAttributes*)attr)->GetX(),((SpectrPluginAttributes*)attr)->GetY());//переделать атрибуты
}

