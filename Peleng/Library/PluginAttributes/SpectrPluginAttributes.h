#ifndef SPECTRPLUGINATTRIBUTES_H
#define SPECTRPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include "../Library/Types.h"
#include <QMap>
#include <QString>

class SpectrPluginAttributes : public IAttributes
{
public:
    SpectrPluginAttributes(u::uint32 x, u::uint32 y, QMap<QString, QString> mapPlugins);
    virtual ~SpectrPluginAttributes();

    u::uint32 GetX() const;
    u::uint32 GetY() const;
    QMap<QString, QString> GetListOfAvaliablePlugins() const;
private:
    u::uint32 m_x;
    u::uint32 m_y;
    QMap<QString, QString> m_mapPlugins;
};

#endif // SPECTRPLUGINATTRIBUTES_H
