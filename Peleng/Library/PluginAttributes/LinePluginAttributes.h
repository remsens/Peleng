#ifndef LINEPLUGINATTRIBUTES_H
#define LINEPLUGINATTRIBUTES_H

#include "IAttributes.h"

#include <QMap>
#include <QString>
#include "../Library/Types.h"

class LinePluginAttributes : public IAttributes
{
public:
    LinePluginAttributes(u::uint32 x1, u::uint32 x2, u::uint32 y1, u::uint32 y2, u::uint32 z1, u::uint32 z2, QMap<QString, QString> mapPlugins);
    virtual ~LinePluginAttributes();
    u::uint32 GetX1() const;
    u::uint32 GetX2() const;
    u::uint32 GetY1() const;
    u::uint32 GetY2() const;
    u::uint32 GetZ1() const;
    u::uint32 GetZ2() const;

    QMap<QString, QString> GetListOfAvaliablePlugins() const;

private:
    u::uint32 m_x1;
    u::uint32 m_x2;
    u::uint32 m_y1;
    u::uint32 m_y2;
    u::uint32 m_z1;
    u::uint32 m_z2;
    QMap<QString, QString> m_mapPlugins;
};

#endif //
