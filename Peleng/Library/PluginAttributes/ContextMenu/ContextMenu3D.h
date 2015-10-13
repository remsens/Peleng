#ifndef CONTEXTMENU3D_H
#define CONTEXTMENU3D_H

#include "BaseContextMenu.h"

class ContextMenu3D : public BaseContextMenu
{
public:
    ContextMenu3D();
    virtual ~ContextMenu3D();

    virtual QList<ContextMenu> GetContextMenuPlugin(const QMap<QString, QString>& availablePlugins);
};

#endif // CONTEXTMENU3D_H
