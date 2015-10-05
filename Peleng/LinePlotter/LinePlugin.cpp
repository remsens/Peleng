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
    w->plotSpectrLine(cube,((LinePluginAttributes*)attr)->GetX1(),((LinePluginAttributes*)attr)->GetX2(),
                           ((LinePluginAttributes*)attr)->GetY1(),((LinePluginAttributes*)attr)->GetY2(),
                           ((LinePluginAttributes*)attr)->GetZ1(),((LinePluginAttributes*)attr)->GetZ2()
                      );
}

