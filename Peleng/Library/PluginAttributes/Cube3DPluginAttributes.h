#ifndef CUBE3DPLUGINATTRIBUTES_H
#define CUBE3DPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include <QMap>
#include <QString>

class Cube3DPluginAttributes: public IAttributes
{
public:
    Cube3DPluginAttributes(QList<ContextMenu> contextMenuList);
    virtual ~Cube3DPluginAttributes();

    QList<ContextMenu> GetListOfAvaliablePlugins() const;

private:
    QList<ContextMenu> m_contextMenuList;
};

#endif // CUBE3DPLUGINATTRIBUTES_H
