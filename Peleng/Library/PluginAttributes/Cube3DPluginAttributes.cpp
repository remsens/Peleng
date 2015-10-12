#include "Cube3DPluginAttributes.h"

Cube3DPluginAttributes::Cube3DPluginAttributes(QList<ContextMenu> contextMenuList)
    : m_contextMenuList(contextMenuList)
{

}

Cube3DPluginAttributes::~Cube3DPluginAttributes()
{

}

QList<ContextMenu> Cube3DPluginAttributes::GetListOfAvaliablePlugins() const
{
    return m_contextMenuList;
}
