#include "LinePluginAttributes.h"

LinePluginAttributes::LinePluginAttributes(u::uint32 x1, u::uint32 x2, u::uint32 y1, u::uint32 y2, u::uint32 z1, u::uint32 z2, QList<ContextMenu> contextMenuList )
    : m_x1(x1)
    , m_x2(x2)
    , m_y1(y1)
    , m_y2(y2)
    , m_z1(z1)
    , m_z2(z2)
    , m_contextMenuList(contextMenuList)
{

}

LinePluginAttributes::~LinePluginAttributes()
{

}

QList<ContextMenu> LinePluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_contextMenuList;
}
u::uint32 LinePluginAttributes::GetX1() const
{
    return m_x1;
}

u::uint32 LinePluginAttributes::GetX2() const
{
    return m_x2;
}

u::uint32 LinePluginAttributes::GetY1() const
{
    return m_y1;
}

u::uint32 LinePluginAttributes::GetY2() const
{
    return m_y2;
}

u::uint32 LinePluginAttributes::GetZ1() const
{
    return m_z1;
}

u::uint32 LinePluginAttributes::GetZ2() const
{
    return m_z2;
}


