#ifndef CHANNELPLUGINATTRIBUTES_H
#define CHANNELPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include "Types.h"

class ChannelPluginAttributes : public IAttributes
{
public:
    ChannelPluginAttributes(u::uint32 channel);
    virtual ~ChannelPluginAttributes();

    u::uint32 GetChannel() const;
private:
   u::uint32 m_channel;
};

#endif // CHANNELPLUGINATTRIBUTES_H
