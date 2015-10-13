#include "PelengPluginLoader.h"

PelengPluginLoader::PelengPluginLoader()
{

}
 PelengPluginLoader::~PelengPluginLoader()
 {

 }

PelengPluginsInterface* PelengPluginLoader::LoadPlugin(const QString& pluginName)
{
    return  qobject_cast<PelengPluginsInterface *>(GetPluginObject(pluginName));
}
