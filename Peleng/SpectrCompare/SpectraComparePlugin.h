#ifndef SPECTRACOMPAREPLUGIN_H
#define SPECTRACOMPAREPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"

#include "SpectraCompare.h"

class SpectraComparePlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "SpectraCompare.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    SpectraComparePlugin(QObject *parent = 0);
    virtual ~SpectraComparePlugin();

private:
    void Execute(HyperCube* cube, Attributes* attr);
    QObject* GetObjectPointer();

private:
    SpectraCompare* m_spectraCompareWindow;
};

#endif // SPECTRACOMPAREPLUGIN_H
