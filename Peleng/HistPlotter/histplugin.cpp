#include "histplugin.h"



HistPlugin::HistPlugin( QObject *parent)
    : QObject(parent)
{

}

HistPlugin::~HistPlugin()
{
    for (int i = 0; i < m_listWindows.size(); i++)
    {
        m_listWindows.at(i)->DestroyCustomPlot();
        delete m_listWindows.at(i);
        m_listWindows.removeAt(i);
    }
}

QObject* HistPlugin::GetObjectPointer()
{
    return this;
}

void HistPlugin::OnClose(HistPlotter* w)
{
    for (int i = 0; i < m_listWindows.size(); i++)
    {
        if (m_listWindows.at(i) == w)
        {
            delete m_listWindows.at(i);
            m_listWindows.removeAt(i);
        }
    }
}

void HistPlugin::Execute(HyperCube *cube, Attributes *attr)
{
    // TODO
    // проверять, построена уже такая гистограмма или нет
    HistPlotter *plotter= new HistPlotter(cube, attr);//TODO
    connect(plotter, SIGNAL(Close(HistPlotter*)), this, SLOT(OnClose(HistPlotter*)));
    plotter->Plot();
    m_listWindows.append(plotter);

}
