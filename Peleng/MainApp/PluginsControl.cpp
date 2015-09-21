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

}

QMap<QString, FileReadInterface*> PluginsControl::GetReadPlugins() const
{
    return m_readPlugins;
}

QMap<QString, PelengPluginsInterface*> PluginsControl::GetPelengPlugins() const
{
    return m_pelengPlugins;
}

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
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if (!fileName.endsWith(".dll")) continue;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QJsonObject MetaData =  pluginLoader.metaData()["MetaData"].toObject();
        if (MetaData["Type"].toString().contains("FileFormat"))
        {
            try {
                QObject *plugin = pluginLoader.instance();
                if (plugin)
                {
                    m_readPlugins.insert(MetaData["Name"].toString(), qobject_cast<FileReadInterface *>(plugin));
                }
            }
            catch (...)
            {
                // TODO
                // надо подумать, нужна тут обработка ошибок или нет
                qDebug() << pluginLoader.errorString();
            }
        } else if (MetaData["Type"].toString().contains("PelengFormat"))
        {
            try {
                QObject *plugin = pluginLoader.instance();
                if (plugin)
                {
                    m_pelengPlugins.insert(MetaData["Name"].toString(), qobject_cast<PelengPluginsInterface *>(plugin));
                }
            }
            catch (...)
            {
                // TODO
                qDebug() << pluginLoader.errorString();
            }
        }
    }
}
