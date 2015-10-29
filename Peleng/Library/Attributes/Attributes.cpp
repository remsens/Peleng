#include "Attributes.h"

Attributes* Attributes::m_instance = NULL;

Attributes::Attributes()
{

}
Attributes* Attributes::I()
{
    if (!m_instance)
    {
        m_instance = new Attributes;
    }
    return m_instance;
}

 void Attributes::Destroy()
 {
     if (m_instance)
     {
         delete m_instance;
     }
 }

Attributes::~Attributes()
{

}

void Attributes::SetPointsList(const Point& point)
{
    m_pointsList.append(point);
}

void Attributes::ClearList() {
    m_pointsList.clear();
}

void Attributes::SetPoint(u::uint32 x, u::uint32 y, u::uint32 z)
{
    Point point;
    point.x = x; point.y = y; point.z = z;
    SetPointsList(point);
}

void Attributes::SetAvailablePlugins(const QMap<QString, ProcessingPluginsInterface*> availablePlugins)
{
    m_availablePlugins.clear();
    m_availablePlugins = availablePlugins;
}

const QList<Point>& Attributes::GetPointsList() const {
    return m_pointsList;
}


const QMap<QString, ProcessingPluginsInterface *> &Attributes::GetAvailablePlugins() const
{
    return m_availablePlugins;
}
