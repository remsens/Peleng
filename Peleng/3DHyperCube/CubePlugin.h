#ifndef CUBEPLUGIN_H
#define CUBEPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"
#include "mainwindow.h"

class CubePlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.3dcube" FILE "3DCubePlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

signals:
    void StartOperation(bool EnabledContext);
    void FinishOperation(bool needToUpdate);
public:
    CubePlugin(QObject *parent = 0);
    virtual ~CubePlugin();
private:
    void Execute(HyperCube* cube, Attributes* attr);
    QObject* GetObjectPointer();

public slots:
    void OnClose(MainWindow* window);
    void onStart(bool flag);
    void onFinish(bool flag);
private:
    QList<MainWindow*> m_listWindows;
};

#endif // CUBEPLUGIN_H
