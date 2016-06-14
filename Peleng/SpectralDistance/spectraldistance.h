#ifndef SPECTRALDISTANCE_H
#define SPECTRALDISTANCE_H

#include <QObject>
#include <math.h>
#include <QDebug>
#include <windows.h>
#include "../Library/Interfaces/ProcessingPluginInterface.h"
#include "../Library/CustomPlotForPreview2D/Preview2D.h"
#include "../Library/Attributes/Attributes.h"
#include "SpecDistWindow.h"

class SpectralDistance : public QObject, public ProcessingPluginsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "by.nomrec.hyperview.spectr" FILE "SpectralDistance.json")
    Q_INTERFACES(ProcessingPluginsInterface)

public:
    explicit SpectralDistance(QObject *parent = 0);
    virtual ~SpectralDistance();

private:
    void Execute(HyperCube* cube, Attributes* attr);
    QObject* GetObjectPointer();

signals:
    void progressPercentChanged();

private:
    void CalcEvklidDistance(); //k = row, l = column
    void CalcSpectralAngle(); //k = row, l = column
    void CalcSpectralCorellation(); //k = row, l = column
    double averageSpectralValue(const int _i, const int _j, bool isInverted);
    void Destroy();
private slots:
    void onClosePreview();

public slots:
    void callMethod(int methNumber);
    void selectRange();
    void changeRange(const int range);
private:
    void normSpectr(QVector<double>& dataSpectr);
private:
    SpecDistWindow *m_specWindow;
    Attributes* m_attr;
    HyperCube* m_pHyperCube;
    QVector<QVector<double> > cube_map;

    Preview2D *preview_2d;

    double min_value;
    double max_value;
    int    view_range;

    bool is_cubemap_emty;
    bool is_evklid_distance;
};

#endif // SPECTRALDISTANCE_H
