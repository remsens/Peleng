#ifndef SPECTRPLUGINATTRIBUTES_H
#define SPECTRPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include "HyperCube.h"
#include "Types.h"
#include <QList>
#include <QString>

class SpectrPluginAttributes : public IAttributes
{
public:
    SpectrPluginAttributes(u::uint32 x, u::uint32 y);
    virtual ~SpectrPluginAttributes();

    u::uint32 GetX() const;
    u::uint32 GetY() const;
    QList<QString> GetListOfAvaliablePlugins() const;
private:
    u::uint32 m_x;
    u::uint32 m_y;
    QList<QString> m_listPlugins;
};

#endif // SPECTRPLUGINATTRIBUTES_H
