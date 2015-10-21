#include "Cube3DPluginAttributes.h"

Cube3DPluginAttributes::Cube3DPluginAttributes()
{

}

Cube3DPluginAttributes::~Cube3DPluginAttributes()
{

}

QList<QString> Cube3DPluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_listPlugins;
}
