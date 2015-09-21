#ifndef PLUGINSCONTROL_H
#define PLUGINSCONTROL_H

#include <QMap>
#include "../Library/Interfaces/FileReadInterface.h"
#include "../Library/Interfaces/PelengPluginsInterface.h"

class PluginsControl
{

public:
    PluginsControl();
    virtual ~PluginsControl();

    QMap<QString, FileReadInterface*> GetReadPlugins() const;
    QMap<QString, PelengPluginsInterface*> GetPelengPlugins() const;
    void LoadPlugins();

private:
    QMap<QString, FileReadInterface*> m_readPlugins;
    QMap<QString, PelengPluginsInterface*> m_pelengPlugins;
};

#endif // PLUGINSCONTROL_H
