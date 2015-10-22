
#include "Main2DWindow.h"
#include <QSharedPointer>
#include "../Library/PluginAttributes/Cube2DPluginAttributes.h"
#include "Hyper2dPlugin.h"

Hyper2dPlugin::Hyper2dPlugin(QObject *parent) : QObject(parent)
{

}

Hyper2dPlugin::~Hyper2dPlugin()
{

}

void Hyper2dPlugin::Execute(HyperCube *cube, IAttributes *attr)
{
    QSharedPointer<Main2DWindow> w(new Main2DWindow(cube,((Cube2DPluginAttributes*)attr)->getInitChanel()));
    w->show();
}

