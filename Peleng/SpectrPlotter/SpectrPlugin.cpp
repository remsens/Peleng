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

void SpectrPlugin::Execute(HyperCube* cube, Attributes* attr)
{
    bool plot = false;
    if (m_windowList.size() != 0)
    {
        for (int i = 0; i < m_windowList.size(); i++)
        {
            if (m_windowList.at(i)->getIsHold())
            {
                m_windowList.at(i)->plotSpectr(cube,  attr->GetPointsList().at(0).x,  attr->GetPointsList().at(0).y);
                plot = true;
            }
        }
    }
    if (!plot)
    {
         PlotterWindow* plotterWindow = new PlotterWindow();
         plotterWindow->plotSpectr(cube,  attr->GetPointsList().at(0).x,  attr->GetPointsList().at(0).y);
         m_windowList.append(plotterWindow);
    }

    for (int i = 0; i < m_windowList.size(); i++)
    {
        m_windowList.at(i)->show();
    }
}
