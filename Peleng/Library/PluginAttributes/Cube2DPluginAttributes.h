#ifndef CUBE2DPLUGINATTRIBUTES_H
#define CUBE2DPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include <QList>
#include <QString>
#include "../Library/Types.h"

class Cube2DPluginAttributes: public IAttributes
{
public:
    Cube2DPluginAttributes(u::uint32 initChanel = 0);
    virtual ~Cube2DPluginAttributes();
    QList<QString> GetListOfAvaliablePlugins() const;
    u::uint32 getInitChanel() const;
private:
    QList<QString> m_listPlugins;
    u::uint32 m_initChanel;

};

#endif // CUBE2DPLUGINATTRIBUTES_H
