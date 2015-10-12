#include "SpectrPluginAttributes.h"

SpectrPluginAttributes::SpectrPluginAttributes(u::uint32 x, u::uint32 y, QList<ContextMenu> contextMenuList)
    : m_x(x)
    , m_y(y)
    , m_contextMenuList(contextMenuList)
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

QList<ContextMenu> SpectrPluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_contextMenuList;
}
