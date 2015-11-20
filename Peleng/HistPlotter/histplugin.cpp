#include "histplugin.h"

#include "histplotter.h"

HistPlugin::HistPlugin( QObject *parent)
    : QObject(parent)
{

}

HistPlugin::~HistPlugin()
{

}

QObject* HistPlugin::GetObjectPointer()
{
    return this;
}
void HistPlugin::Execute(HyperCube *cube, Attributes *attr)
{
    HistPlotter *plotter= new HistPlotter(cube, attr);//TODO
    plotter->Plot();

}
