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

void Attributes::SetModeLib(bool value)
{
    m_addSpectr = value;
}

bool Attributes::GetModeLib() const
{
    return m_addSpectr;
}

void Attributes::SetXUnit(double xUnit)
{
    m_XUnits.push_back(xUnit);
}

void Attributes::SetYUnit(double yUnit)
{
    m_YUnits.push_back(yUnit);
}

void Attributes::ClearUnitsLists()
{
    m_XUnits.clear();
    m_YUnits.clear();
    m_descriptionSpectr.clear();
}

void Attributes::SetDescriptionItem(const QString& keyTitle, const QString& value)
{
    DescriptionSpectr ds;
    ds.title = keyTitle;
    ds.description = value;
    m_descriptionSpectr.append(ds);
}

const QVector<double>& Attributes::GetXUnits() const
{
    return m_XUnits;
}

const QVector<double>& Attributes::GetYUnits() const
{
    return m_YUnits;
}

const QList<Attributes::DescriptionSpectr> &Attributes::GetSpectrumDescription() const
{
    return m_descriptionSpectr;
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

void Attributes::SetExternalSpectrFlag(bool externalSpectr)
{
    m_externalSpectr = externalSpectr;
}

bool Attributes::GetExternalSpectrFlag() const
{
    return m_externalSpectr;
}

int  Attributes::GetFormatExternalSpectr() const
{
    return m_formatExternalSpectr;
}

void Attributes::SetExternalSpectrFormat(int format)
{
    m_formatExternalSpectr = format;
}
