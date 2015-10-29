#ifndef PLUGINSCONTROL_H
#define PLUGINSCONTROL_H

// в классе будет подгружать все плагины, которые есть
// возможно, нужна будет оптимизация, поэтому классы ProcessingPluginLoader и ReadinPluginLoader не удаляются

#include <QMap>
#include <QString>
#include "../Library/Interfaces/FileReadInterface.h"
#include "../Library/Interfaces/ProcessingPluginInterface.h"

class PluginsControl
{

public:
    PluginsControl();
    virtual ~PluginsControl();

    // В функции мы только получаем названия плагинов. При загрузке плагинов происходит создание объекта, что нам пока не нужно
   // void LoadNamesPlugins();
   void LoadPlugins();

   const QMap<QString, FileReadInterface*> GetReadingPlugins() const;
   const QMap<QString, ProcessingPluginsInterface*> GetProcessingPlugins() const;

private:
   QMap<QString, FileReadInterface*> m_readingPluginsMap;
   QMap<QString, ProcessingPluginsInterface*> m_processingPluginsMap;
};

#endif // PLUGINSCONTROL_H
