#ifndef CHANNELPLUGINATTRIBUTES_H
#define CHANNELPLUGINATTRIBUTES_H

#include "IAttributes.h"

#include "Types.h"
#include <QMap>
#include <QString>

class ChannelPluginAttributes : public IAttributes
{
public:
    ChannelPluginAttributes(u::uint32 channel, QMap<QString, QString> mapPlugins); // name, discription
    virtual ~ChannelPluginAttributes();

    u::uint32 GetChannel() const;
    QMap<QString, QString> GetListOfAvaliablePlugins() const;

private:
   u::uint32 m_channel;
   QMap<QString, QString> m_mapPlugins;
};

#endif // CHANNELPLUGINATTRIBUTES_H
