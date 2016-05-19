#ifndef ATMCORRMODULE_H
#define ATMCORRMODULE_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"


class AtmCorrModule : public QObject, public ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.remsens.hyperview.atmcorr" FILE "AtmoshericCorrection.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    AtmCorrModule(QObject *parent = 0);
    virtual ~AtmCorrModule();

    void Execute(HyperCube *cube, Attributes *attr);
    QObject* GetObjectPointer();
};

#endif // ATMCORRMODULE_H
