#include "Cube2DPluginAttributes.h"

Cube2DPluginAttributes::Cube2DPluginAttributes(u::uint32 initChanel)
    : m_initChanel(initChanel)
{

}

Cube2DPluginAttributes::~Cube2DPluginAttributes()
{

}

QList<QString> Cube2DPluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_listPlugins;
}

u::uint32 Cube2DPluginAttributes::getInitChanel() const
{
    return m_initChanel;
}
