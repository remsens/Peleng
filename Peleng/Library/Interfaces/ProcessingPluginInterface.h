#include "../Library/Attributes/Attributes.h"

#ifndef PROCESSINGPLUGININTERFACE
#define PROCESSINGPLUGININTERFACE


#include "../Library/HyperCube.h"


class ProcessingPluginsInterface
{
public:
    virtual void Execute(HyperCube* cube, Attributes* attr) = 0;
};

#define ProcessingPluginsInterface_iid  "by.nomrec.ProcessingPluginsInterface"
Q_DECLARE_INTERFACE(ProcessingPluginsInterface, ProcessingPluginsInterface_iid)



#endif // PROCESSINGPLUGININTERFACE

