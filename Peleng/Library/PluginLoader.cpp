#include "PluginLoader.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QJsonObject>
#include <QDebug>

PluginLoader::PluginLoader()
{

}

PluginLoader::~PluginLoader()
{

}

QObject* PluginLoader::GetPluginObject(const QString &pluginName)
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
        if (MetaData["Name"].toString().compare(pluginName) == 0)
        {
            try {
                return pluginLoader.instance();

            }
            catch (...)
            {
                // TODO
                // надо подумать, нужна тут обработка ошибок или нет
                qDebug() << pluginLoader.errorString();
            }
        }
    }
    qDebug() << "Plugin didn't load";
    return 0;
}
