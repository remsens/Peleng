#include "BaseContextMenu.h"
#include "Types.h"

BaseContextMenu::BaseContextMenu()
{

}
BaseContextMenu::~BaseContextMenu()
{

}

QList<ContextMenu> BaseContextMenu::CreateContextMenu(const QList<ContextMenu>& contextPluginMenu, const QMap<QString, QString>& avaliablePlugins)
{
    QList<ContextMenu> m_ContextPluginMenu;

    if (m_ContextPluginMenu.size() != 0)
    {
        m_ContextPluginMenu.clear();
    }
    for (u::uint8 i = 0; i < contextPluginMenu.size(); i++)
    {
        if (avaliablePlugins.contains(contextPluginMenu.at(i).namePlugin))
        {
            m_ContextPluginMenu.append(contextPluginMenu.at(i));
        }
    }
    return m_ContextPluginMenu;
}
