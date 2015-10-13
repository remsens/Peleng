#ifndef CUBE3DPLUGINATTRIBUTES_H
#define CUBE3DPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include <QMap>
#include <QString>

class Cube3DPluginAttributes: public IAttributes
{
public:
    Cube3DPluginAttributes(QMap<QString, QString> pluginList);
    virtual ~Cube3DPluginAttributes();

    QMap<QString, QString> GetListOfAvaliablePlugins() const;

private:
    QMap<QString, QString> m_contextPluginList;
};

#endif // CUBE3DPLUGINATTRIBUTES_H
