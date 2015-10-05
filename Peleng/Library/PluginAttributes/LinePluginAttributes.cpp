#include "LinePluginAttributes.h"

LinePluginAttributes::LinePluginAttributes(u::uint32 x, u::uint32 y)
    : m_x(x)
    , m_y(y)
{

}

LinePluginAttributes::~LinePluginAttributes()
{

}

u::uint32 LinePluginAttributes::GetX() const
{
    return m_x;
}

u::uint32 LinePluginAttributes::GetY() const
{
    return m_y;
}
