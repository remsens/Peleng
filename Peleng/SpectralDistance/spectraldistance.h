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
#include "../Library/Attributes/Attributes.h"


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
    void CalcEvklidDistance(const QVector<double> &xArr, const QVector<double> &yArr); //k = row, l = column
    void CalcSpectralAngle(const QVector<double> &xArr,const QVector<double>& yArr); //k = row, l = column
    void CalcSpectralCorellation(const QVector<double>& xArr,const QVector<double>& yArr); //k = row, l = column
    double averageSpectralValue(const int _i, const int _j, bool isInverted);
    double averageSpectralValue(QVector<double> yArr, int chan_count);
    void Destroy();
private slots:
    void onClosePreview();

public slots:
    void callMethod(int methNumber);
    void selectRange();
    void changeRange(const int range);
    void OnCloseEvent(QQuickCloseEvent*);
private:
    QQmlApplicationEngine* engine;
    QQuickWindow *window;
    Attributes* m_attr;
    HyperCube* m_pHyperCube;
    QVector<QVector<double> > cube_map;
    QVector<double> m_xArr; //x спектра, с которым сравниваем остальные
    QVector<double> m_yArr; //y спектра, с которым сравниваем остальные
    Preview2D *preview_2d;

    double min_value;
    double max_value;
    int    view_range;

    bool is_cubemap_emty;
    bool is_evklid_distance;
};

#endif // SPECTRALDISTANCE_H
