#ifndef CHANNELPLUGINATTRIBUTES_H
#define CHANNELPLUGINATTRIBUTES_H

#include "IAttributes.h"

#include "Types.h"
#include <QMap>
#include <QString>

class ChannelPluginAttributes : public IAttributes
{
public:
    ChannelPluginAttributes(u::uint32 channel, QList<ContextMenu> contextMenuList); // name, discription
    virtual ~ChannelPluginAttributes();

    u::uint32 GetChannel() const;
    QList<ContextMenu> GetListOfAvaliablePlugins() const;

private:
   u::uint32 m_channel;
   QList<ContextMenu> m_contextMenuList;
};

#endif // CHANNELPLUGINATTRIBUTES_H
