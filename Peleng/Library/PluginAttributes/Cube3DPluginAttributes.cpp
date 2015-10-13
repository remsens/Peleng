#include "Cube3DPluginAttributes.h"

Cube3DPluginAttributes::Cube3DPluginAttributes(QMap<QString, QString> pluginList)
    : m_contextPluginList(pluginList)
{

}

Cube3DPluginAttributes::~Cube3DPluginAttributes()
{

}

QMap<QString, QString> Cube3DPluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_contextPluginList;
}
