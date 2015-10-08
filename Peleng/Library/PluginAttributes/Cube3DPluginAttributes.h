#ifndef CUBE3DPLUGINATTRIBUTES_H
#define CUBE3DPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include <QMap>
#include <QString>

class Cube3DPluginAttributes: public IAttributes
{
public:
    Cube3DPluginAttributes(QMap<QString, QString> mapPlugins);
    virtual ~Cube3DPluginAttributes();

    QMap<QString, QString> GetListOfAvaliablePlugins() const;
private:
    QMap<QString, QString> m_mapPlugins;
};

#endif // CUBE3DPLUGINATTRIBUTES_H
