#ifndef SPECTRALDISTANCE_H
#define SPECTRALDISTANCE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <math.h>
#include <QDebug>
#include <windows.h>
#include "../Library/Interfaces/ProcessingPluginInterface.h"
#include "../Library/CustomPlotForPreview2D/Preview2D.h"


class SpectralDistance : public QObject, public ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "SpectralDistance.json")
    Q_INTERFACES(ProcessingPluginsInterface)

    QQmlApplicationEngine* engine;
    QQuickWindow *window;

    HyperCube* m_pHyperCube;
    QVector<QVector<double> > cube_map;
    QVector<QVector<int> >    masked_map;
    std::vector<std::vector<double> > new_map;

    Preview2D *preview_2d;

    double min_value;
    double max_value;

    bool is_cubemap_emty;

public:
    explicit SpectralDistance(QObject *parent = 0);
    virtual ~SpectralDistance();

signals:
    void progressPercentChanged();


private:
    void Execute(HyperCube* cube, Attributes* attr);
    void CalcEvklidDistance();
    void CalcSpectralAngle();
    void CalcSpectralCorellation();
    double averageSpectralValue(const int _i, const int _j);
    QObject* GetObjectPointer();
public slots:
    void callMethod(int methNumber);
    void selectRange(const double percent);
};

#endif // SPECTRALDISTANCE_H
