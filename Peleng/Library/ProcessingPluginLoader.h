#ifndef PROCESSINGPLUGINLOADER_H
#define PROCESSINGPLUGINLOADER_H

#include "PluginLoader.h"
#include "../Library/Interfaces/ProcessingPluginInterface.h"

class ProcessingPluginLoader : public PluginLoader
{
public:
    ProcessingPluginLoader();
    virtual ~ProcessingPluginLoader();

    ProcessingPluginsInterface* LoadPlugin(const QString& pluginName);

};

#endif // PROCESSINGPLUGINLOADER_H
