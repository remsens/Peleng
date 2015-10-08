#include "Cube3DPluginAttributes.h"

Cube3DPluginAttributes::Cube3DPluginAttributes(QMap<QString, QString> mapPlugins)
    : m_mapPlugins(mapPlugins)
{

}

Cube3DPluginAttributes::~Cube3DPluginAttributes()
{

}

QMap<QString, QString> Cube3DPluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_mapPlugins;
}
