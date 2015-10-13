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

    QMap<QString, QString> GetReadPlugins() const;
    QMap<QString, QString> GetPelengPlugins() const;
    // В функции мы только получаем названия плагинов. При загрузке плагинов происходит создание объекта, что нам пока не нужно
    void LoadNamesPlugins();

private:
    QMap<QString, QString> m_readPluginsNames;
    QMap<QString, QString> m_pelengPluginsNames;
};

#endif // PLUGINSCONTROL_H
