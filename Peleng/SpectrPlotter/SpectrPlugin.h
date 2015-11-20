#ifndef SPECTRPLUGIN_H
#define SPECTRPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"
#include "PlotterWindow.h"
#include <QList>

class SpectrPlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "SpectrPlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    SpectrPlugin(QObject *parent = 0);
    virtual ~SpectrPlugin();

private:
    void Execute(HyperCube* cube, Attributes* attr);
    QObject* GetObjectPointer();
public slots:
    void OnClose(PlotterWindow *plotterWindow);
private:
    QList<PlotterWindow*> m_windowList;
    //PlotterWindow* m_plotterWindow;

};

#endif
