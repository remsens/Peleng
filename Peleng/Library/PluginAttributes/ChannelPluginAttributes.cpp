#include "ChannelPluginAttributes.h"

ChannelPluginAttributes::ChannelPluginAttributes(u::uint32 channel, QList<QString> listPlugins)
    : m_channel(channel)
    , m_listPlugins(listPlugins)
{

}

ChannelPluginAttributes::~ChannelPluginAttributes()
{

}

u::uint32 ChannelPluginAttributes::GetChannel() const
{
    return m_channel;
}

QList<QString> ChannelPluginAttributes::GetListOfAvaliablePlugins() const
 {
     return m_listPlugins;
 }
