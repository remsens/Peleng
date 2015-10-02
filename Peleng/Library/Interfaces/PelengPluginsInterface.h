#ifndef PELENGPLUGINSINTERFACE
#define PELENGPLUGINSINTERFACE

#include "../../Library/HyperCube.h"
#include "../../Library/PluginAttributes/IAttributes.h"

class PelengPluginsInterface
{
public:
    virtual void Execute(HyperCube* cube, IAttributes* attr=0) = 0;
};

#define PelengPluginsInterface_iid "by.nomrec.PelengPluginsInterface"
Q_DECLARE_INTERFACE(PelengPluginsInterface, PelengPluginsInterface_iid)
#endif // PELENGPLUGINSINTERFACE

