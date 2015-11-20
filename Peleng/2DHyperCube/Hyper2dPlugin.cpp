#include "Hyper2dPlugin.h"

#include "Main2DWindow.h"

Hyper2dPlugin::Hyper2dPlugin(QObject *parent) : QObject(parent)
{

}

Hyper2dPlugin::~Hyper2dPlugin()
{

}

QObject* Hyper2dPlugin::GetObjectPointer()
{
    return this;
}

void Hyper2dPlugin::Execute(HyperCube *cube, Attributes *attr)
{
    Main2DWindow* w = new Main2DWindow(cube, attr);
    w->show();
}

