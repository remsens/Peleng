#include "ChannelPluginAttributes.h"

ChannelPluginAttributes::ChannelPluginAttributes(u::uint32 channel, QList<ContextMenu> contextMenuList)
    : m_channel(channel)
    , m_contextMenuList(contextMenuList)
{

}

ChannelPluginAttributes::~ChannelPluginAttributes()
{

}

u::uint32 ChannelPluginAttributes::GetChannel() const
{
    return m_channel;
}

QList<ContextMenu> ChannelPluginAttributes::GetListOfAvaliablePlugins() const
 {
     return m_contextMenuList;
 }
