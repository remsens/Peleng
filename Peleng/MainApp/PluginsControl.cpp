#include "PluginsControl.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QJsonObject>
#include <QDebug>


PluginsControl::PluginsControl()
{

}

PluginsControl::~PluginsControl()
{
    m_readingPluginsMap.clear();
    m_processingPluginsMap.clear();
}


const QMap<QString, FileReadInterface*> PluginsControl::GetReadingPlugins() const
{
    return m_readingPluginsMap;
}

const QMap<QString, ProcessingPluginsInterface*> PluginsControl::GetProcessingPlugins() const
{
    return m_processingPluginsMap;
}

// подгружаем плагины сразу в память
void PluginsControl::LoadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
       // pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
   // pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if (!fileName.endsWith(".dll")) continue;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QJsonObject MetaData =  pluginLoader.metaData()["MetaData"].toObject();
        if (MetaData["Type"].toString().contains("FileFormat"))
        {
            try
            {
               FileReadInterface*  obj = qobject_cast<FileReadInterface *>(pluginLoader.instance());
               m_readingPluginsMap.insert(MetaData["Name"].toString(), obj);

            }
            catch (...)
            {
                // TODO
                // надо подумать, нужна тут обработка ошибок или нет
                qDebug() << pluginLoader.errorString();
            }

        } else if (MetaData["Type"].toString().contains("ProcessingFormat"))
        {
            try
            {
               ProcessingPluginsInterface*  obj = qobject_cast<ProcessingPluginsInterface *>(pluginLoader.instance());
               m_processingPluginsMap.insert(MetaData["Name"].toString(), obj);

            }
            catch (...)
            {
                // TODO
                // надо подумать, нужна тут обработка ошибок или нет
                qDebug() << pluginLoader.errorString();
            }
        }
    }
    qDebug() << "FilePlugins" << m_readingPluginsMap.size();
    qDebug() << "ProcessingPlugins" << m_processingPluginsMap.size();
}

/*void PluginsControl::LoadNamesPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
       // pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
   // pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if (!fileName.endsWith(".dll")) continue;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QJsonObject MetaData =  pluginLoader.metaData()["MetaData"].toObject();
        if (MetaData["Type"].toString().contains("FileFormat"))
        {

           ReadingPlugins.append(MetaData["Name"].toString());

        } else if (MetaData["Type"].toString().contains("PelengFormat"))
        {
            ProcessingPlugins.append(MetaData["Name"].toString());
        }
    }
    qDebug() << "FilePlugins" << ReadingPlugins.size();
    qDebug() << "ProcessingPlugins" << ProcessingPlugins.size();
}*/
