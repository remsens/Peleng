#ifndef NOISEPLUGIN_H
#define NOISEPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"


class NoisePlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "NoisePlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

signals:
    void StartOperation(bool EnabledContext);
    void FinishOperation(bool needToUpdate);

public:
    explicit NoisePlugin(QObject *parent = 0);
    virtual ~NoisePlugin();

private:
    void Execute(HyperCube* cube, Attributes* attr);
    QObject* GetObjectPointer();
};

#endif // NOISEPLUGIN_H
