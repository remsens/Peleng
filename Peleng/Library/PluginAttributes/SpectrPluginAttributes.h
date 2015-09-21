#ifndef SPECTRPLUGINATTRIBUTES_H
#define SPECTRPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include "HyperCube.h"
#include "Types.h"

class SpectrPluginAttributes : public IAttributes
{
public:
    SpectrPluginAttributes(u::uint32 x, u::uint32 y);
    virtual ~SpectrPluginAttributes();

    u::uint32 GetX() const;
    u::uint32 GetY() const;

private:
    u::uint32 m_x;
    u::uint32 m_y;
};

#endif // SPECTRPLUGINATTRIBUTES_H
