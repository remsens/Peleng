#ifndef HISTPLUGIN_H
#define HISTPLUGIN_H

#include <QObject>
#include "../Library/Interfaces/ProcessingPluginInterface.h"

//#include "histplotter.h"
#include "histplotterwidget.h"

class HistPlugin : public QObject, public  ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.hist" FILE "HistPlugin.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    HistPlugin(QObject *parent = 0);
    virtual ~HistPlugin();


signals:
    void ReplotSignal(Attributes* attr);


public:
    void Execute(HyperCube *cube, Attributes* attr = 0);
    QObject* GetObjectPointer();

public slots:
    void OnClose(HistPlotterWidget* w);



private:
    QList<HistPlotterWidget*> m_listWindows;
};

#endif // HISTPLUGIN_H
