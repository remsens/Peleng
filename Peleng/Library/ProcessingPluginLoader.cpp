#include "ProcessingPluginLoader.h"

ProcessingPluginLoader::ProcessingPluginLoader()
{

}

ProcessingPluginLoader::~ProcessingPluginLoader()
{

}

ProcessingPluginsInterface* ProcessingPluginLoader::LoadPlugin(const QString& pluginName)
{
   return  qobject_cast<ProcessingPluginsInterface *>(GetPluginObject(pluginName));
}
