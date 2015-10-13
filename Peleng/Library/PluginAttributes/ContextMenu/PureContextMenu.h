#ifndef PURECONTEXTMENU_H
#define PURECONTEXTMENU_H

#include "BaseContextMenu.h"

class PureContextMenu : public BaseContextMenu
{
public:
    PureContextMenu();
    virtual ~PureContextMenu();

    virtual QList<ContextMenu> GetContextMenuPlugin(const QMap<QString, QString>& availablePlugins);
};
#endif // PURECONTEXTMENU_H
