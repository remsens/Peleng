#ifndef SPECTRPLUGINATTRIBUTES_H
#define SPECTRPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include "../Library/Types.h"
#include <QMap>
#include <QString>

class SpectrPluginAttributes : public IAttributes
{
public:
    SpectrPluginAttributes(u::uint32 x, u::uint32 y, QList<ContextMenu> contextMenuList);
    virtual ~SpectrPluginAttributes();

    u::uint32 GetX() const;
    u::uint32 GetY() const;
    QList<ContextMenu> GetListOfAvaliablePlugins() const;
private:
    u::uint32 m_x;
    u::uint32 m_y;
    QList<ContextMenu> m_contextMenuList;
};

#endif // SPECTRPLUGINATTRIBUTES_H
