#include "ReadPluginLoader.h"

ReadPluginLoader::ReadPluginLoader()
{

}
ReadPluginLoader::~ReadPluginLoader()
{

}

FileReadInterface* ReadPluginLoader::LoadPlugin(const QString& pluginName)
{
    return qobject_cast<FileReadInterface *>(GetPluginObject(pluginName));
}
