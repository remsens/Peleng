#include "SpectrPluginAttributes.h"

SpectrPluginAttributes::SpectrPluginAttributes(u::uint32 x, u::uint32 y, QMap<QString, QString> mapPlugins)
    : m_x(x)
    , m_y(y)
    , m_mapPlugins(mapPlugins)
{

}

SpectrPluginAttributes::~SpectrPluginAttributes()
{

}

u::uint32 SpectrPluginAttributes::GetX() const
{
    return m_x;
}

u::uint32 SpectrPluginAttributes::GetY() const
{
    return m_y;
}

QMap<QString, QString> SpectrPluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_mapPlugins;
}
