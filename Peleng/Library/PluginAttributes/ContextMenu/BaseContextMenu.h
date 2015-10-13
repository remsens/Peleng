#ifndef BASECONTEXTMENU_H
#define BASECONTEXTMENU_H

#include "icontextmenu.h"
#include <QMap>

class BaseContextMenu : public IContextPluginMenu
{
public:
    BaseContextMenu();
    virtual ~BaseContextMenu();

protected:
    QList<ContextMenu> CreateContextMenu(const QList<ContextMenu> &contextPluginMenu, const QMap<QString, QString>& avaliablePlugins);

protected:


};

#endif // BASECONTEXTMENU_H
