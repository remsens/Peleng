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

QMap<QString, QString> PluginsControl::GetReadPlugins() const
{
    return m_readPluginsNames;
}

QMap<QString, QString> PluginsControl::GetPelengPlugins() const
{
    return m_pelengPluginsNames;
}


void PluginsControl::LoadNamesPlugins()
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
            m_readPluginsNames.insert(MetaData["Name"].toString(), MetaData["Description"].toString());

        } else if (MetaData["Type"].toString().contains("PelengFormat"))
        {
            m_pelengPluginsNames.insert(MetaData["Name"].toString(), MetaData["Description"].toString());
        }
    }
    qDebug() << "FilePlugins" << m_readPluginsNames.size();
    qDebug() << "PelengPlugins" << m_pelengPluginsNames.size();
}
