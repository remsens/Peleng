#ifndef READPLUGINLOADER_H
#define READPLUGINLOADER_H

#include "PluginLoader.h"
#include "../Library/Interfaces/FileReadInterface.h"

class ReadPluginLoader : public PluginLoader
{
public:
    ReadPluginLoader();
    virtual ~ReadPluginLoader();

    FileReadInterface* LoadPlugin(const QString& pluginName);
};

#endif // READPLUGINLOADER_H
