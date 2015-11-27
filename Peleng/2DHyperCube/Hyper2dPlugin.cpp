#include "Hyper2dPlugin.h"

Hyper2dPlugin::Hyper2dPlugin(QObject *parent) : QObject(parent)
{

}

Hyper2dPlugin::~Hyper2dPlugin()
{
    for (int i = 0; i < m_listWindows.size(); i++)
    {
        delete m_listWindows.at(i);
        m_listWindows.removeAt(i);
    }
}

QObject* Hyper2dPlugin::GetObjectPointer()
{
    return this;
}

void Hyper2dPlugin::OnClose(Main2DWindow* window2D)
{
    for (int i = 0; i < m_listWindows.size(); i++)
    {
        if (m_listWindows.at(i) == window2D)
        {
            m_listWindows.removeAt(i);
        }
    }
}

void Hyper2dPlugin::Execute(HyperCube *cube, Attributes *attr)
{
    Main2DWindow* w = new Main2DWindow(cube, attr);
    QObject::connect(w, SIGNAL(CloseWindow(Main2DWindow*)), this, SLOT(OnClose(Main2DWindow*)));
    m_listWindows.append(w);
    w->show();
}

