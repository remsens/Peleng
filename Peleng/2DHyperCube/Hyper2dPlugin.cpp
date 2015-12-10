#include "Hyper2dPlugin.h"

#include <QDebug>

Hyper2dPlugin::Hyper2dPlugin(QObject *parent) : QObject(parent)
{
    m_window = NULL;
}

Hyper2dPlugin::~Hyper2dPlugin()
{
    if (m_window != NULL)
    {
        delete m_window;
    }
}

QObject* Hyper2dPlugin::GetObjectPointer()
{
    return this;
}

void Hyper2dPlugin::OnClose()
{
    m_window = NULL;
}

void Hyper2dPlugin::Execute(HyperCube *cube, Attributes *attr)
{
    if (m_window == NULL)
    {
        m_window = new Main2DWindow(cube, attr);
        QObject::connect(m_window, SIGNAL(CloseWindow()), this, SLOT(OnClose()));
    }
    m_window->show();
    m_window->resize(m_window->size().width(),m_window->size().height() + 1);
    m_window->activateWindow();
    m_window->raise();
    m_window->showNormal();// если окно было свернуто
}

