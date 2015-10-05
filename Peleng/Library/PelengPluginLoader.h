#ifndef PELENGPLUGINLOADER_H
#define PELENGPLUGINLOADER_H

#include "PluginLoader.h"
#include "../Library/Interfaces/PelengPluginsInterface.h"
class PelengPluginLoader : public PluginLoader
{
public:
    PelengPluginLoader();
    virtual ~PelengPluginLoader();

    PelengPluginsInterface* LoadPlugin(const QString& pluginName);
};

#endif // PELENGPLUGINLOADER_H
