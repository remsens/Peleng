#ifndef LINEPLUGINATTRIBUTES_H
#define LINEPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include "HyperCube.h"
#include "Types.h"

class LinePluginAttributes : public IAttributes
{
public:
    LinePluginAttributes(u::uint32 x, u::uint32 y);
    virtual ~LinePluginAttributes();

    u::uint32 GetX() const;
    u::uint32 GetY() const;

private:
    u::uint32 m_x;
    u::uint32 m_y;
};

#endif //
