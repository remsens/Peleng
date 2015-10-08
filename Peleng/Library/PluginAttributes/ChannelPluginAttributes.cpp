#include "ChannelPluginAttributes.h"

ChannelPluginAttributes::ChannelPluginAttributes(u::uint32 channel, QMap<QString, QString> listPlugins)
    : m_channel(channel)
    , m_mapPlugins(listPlugins)
{

}

ChannelPluginAttributes::~ChannelPluginAttributes()
{

}

u::uint32 ChannelPluginAttributes::GetChannel() const
{
    return m_channel;
}

QMap<QString, QString> ChannelPluginAttributes::GetListOfAvaliablePlugins() const
 {
     return m_mapPlugins;
 }
