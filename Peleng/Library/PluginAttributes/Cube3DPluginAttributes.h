#ifndef CUBE3DPLUGINATTRIBUTES_H
#define CUBE3DPLUGINATTRIBUTES_H

#include "IAttributes.h"
#include <QList>
#include <QString>

class Cube3DPluginAttributes: public IAttributes
{
public:
    Cube3DPluginAttributes();
    virtual ~Cube3DPluginAttributes();
    QList<QString> GetListOfAvaliablePlugins() const;
private:
    QList<QString> m_listPlugins;
};

#endif // CUBE3DPLUGINATTRIBUTES_H
