#ifndef CHANNELPLUGINATTRIBUTES_H
#define CHANNELPLUGINATTRIBUTES_H

#include "IAttributes.h"

#include "../Types.h"
#include <QList>
#include <QString>

class ChannelPluginAttributes : public IAttributes
{
public:
    ChannelPluginAttributes(u::uint32 channel);
    virtual ~ChannelPluginAttributes();

    u::uint32 GetChannel() const;
   // QList<QString> GetListOfAvaliablePlugins() const;
private:
   u::uint32 m_channel;
  // QList<QString> m_listPlugins;
};
#endif // CHANNELPLUGINATTRIBUTES_H
