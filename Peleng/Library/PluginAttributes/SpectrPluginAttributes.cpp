#include "SpectrPluginAttributes.h"

SpectrPluginAttributes::SpectrPluginAttributes(u::uint32 x, u::uint32 y)
    : m_x(x)
    , m_y(y)
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
