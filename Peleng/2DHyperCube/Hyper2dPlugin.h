#ifndef HYPERTWODPLUGIN_H
#define HYPERTWODPLUGIN_H

#include <QObject>
#include <QFile>
#include "Main2DWindow.h"
#include "../Library/Interfaces/ProcessingPluginInterface.h"

class Hyper2dPlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "2DCubePlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)
public:
    explicit Hyper2dPlugin(QObject *parent = 0);
    virtual ~Hyper2dPlugin();

public slots:
    void OnClose();

private:
    void Execute(HyperCube* cube, Attributes* attr);
    QObject* GetObjectPointer();

private:
    //QList<Main2DWindow*> m_listWindows;
    Main2DWindow* m_window;
};

#endif // HYPER2DPLUGIN_H
