#ifndef PLUGINSCONTROL_H
#define PLUGINSCONTROL_H

#include <QMap>
#include "../Library/Interfaces/FileReadInterface.h"
#include "../Library/Interfaces/PelengPluginsInterface.h"

class PluginsControl
{

public:
    PluginsControl();
    virtual ~PluginsControl();

    QList<QString> GetReadPlugins() const;
    QList<QString> GetPelengPlugins() const;
    // В функции мы только получаем названия плагинов. При загрузке плагинов происходит создание объекта, что нам пока не нужно
    void LoadNamesPlugins();

private:
    QList<QString> m_readPluginsNames;
    QList<QString> m_pelengPluginsNames;
};

#endif // PLUGINSCONTROL_H
