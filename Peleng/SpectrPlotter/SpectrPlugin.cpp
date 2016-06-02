#include "SpectrPlugin.h"
#include <qDebug>

#include <QList>

SpectrPlugin::SpectrPlugin(QObject* parent)
    :QObject(parent)
{

}

SpectrPlugin::~SpectrPlugin()
{
   m_windowList.clear();
}

QObject* SpectrPlugin::GetObjectPointer()
{
    return this;
}

void SpectrPlugin::OnClose(PlotterWindow* plotterWindow)
{
    for (int i = 0; i < m_windowList.size(); i++)
    {
        if (m_windowList.at(i) == plotterWindow)
        {
            m_windowList.removeAt(i);
        }
    }
}
void SpectrPlugin::Execute(HyperCube* cube, Attributes* attr)
{
    bool plot = false;
    if (m_windowList.size() != 0)
    {
        for (int i = 0; i < m_windowList.size(); i++)
        {
            if (m_windowList.at(i)->getIsHold())
            {
                 // передавать, отображать подписи осей, или нет
                 m_windowList.at(i)->plotSpectr(attr->GetCurrentSpectr()->GetXUnits(), attr->GetCurrentSpectr()->GetYUnits());
            }
                plot = true;
         }
    }
    if (!plot)
    {
        PlotterWindow* plotterWindow = new PlotterWindow(cube, attr);
        QObject::connect(plotterWindow, SIGNAL(closePlotterWindow(PlotterWindow*)), this, SLOT(OnClose(PlotterWindow*)));
        plotterWindow->plotSpectr(attr->GetCurrentSpectr()->GetXUnits(), attr->GetCurrentSpectr()->GetYUnits());
        m_windowList.append(plotterWindow);
    }

    for (int i = 0; i < m_windowList.size(); i++)
    {
        m_windowList.at(i)->show();
        m_windowList.at(i)->activateWindow();
    }
}
