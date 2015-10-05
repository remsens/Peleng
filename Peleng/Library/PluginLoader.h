#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>

class PluginLoader
{
public:
    PluginLoader();
    virtual ~PluginLoader();
protected:
    // Загрузка плагина
    // Не реализована проверка, относится ли плагин в нужному интерфейсу. Предполагается, что названия плагинов разные
    QObject* GetPluginObject(const QString& pluginName);
};

#endif // PLUGINLOADER_H
