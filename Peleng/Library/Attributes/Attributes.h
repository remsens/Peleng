#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H


#include <QObject>
#include <QMap>
#include <QString>
#include <QList>
#include <../Library/PointStruct.h>


class Attributes;
#include "../Library/Interfaces/ProcessingPluginInterface.h"

enum NoiseAlgorithm {
    Median2D, Median1D
};

class Attributes
{
public:
    static Attributes *I();
    static void Destroy();
    virtual ~Attributes();

    // общее
    void SetPointsList(const Point &point);
    void SetPoint(u::uint32 x, u::uint32 y, u::uint32 z);
    void ClearList();
    void SetAvailablePlugins(const QMap<QString, ProcessingPluginsInterface*> availablePlugins);

    // фундаментальные библиотеки
    void SetModeLib(bool value);
    bool GetModeLib() const;
    void SetXUnit(double xUnit);
    void SetYUnit(double yUnit);
    void SetXUnit(QVector<double>& xUnits);
    void SetYUnit(QVector<double>& yUnits);
    void ClearUnitsLists();
    struct DescriptionSpectr
    {
        QString title;
        QString description;
    };
    void SetDescriptionItem(const QString& keyTitle, const QString& value);
    void SetDescriptionSpectr(QList<DescriptionSpectr>& ds);
    const QVector<double>& GetXUnits() const;
    const QVector<double>& GetYUnits() const;
    const QList<Attributes::DescriptionSpectr>& GetSpectrumDescription() const;

    //SpectrPlotter
    void SetExternalSpectrFlag(bool externalSpectr);
    bool GetExternalSpectrFlag() const;
    void SetExternalSpectrFormat(int format);
    int  GetFormatExternalSpectr() const;

    // общее
    const QList<Point>& GetPointsList() const;
    const QMap<QString, ProcessingPluginsInterface*>& GetAvailablePlugins() const;

    // шумы

    void SetNoiseAlg(NoiseAlgorithm alg);
    NoiseAlgorithm GetNoiseAlg() const;
private:
    Attributes();
    Attributes(const Attributes&);
    Attributes& operator=(Attributes&);

private:
    static Attributes* m_instance;

private:
    QList<Point> m_pointsList;
    QMap<QString, ProcessingPluginsInterface*> m_availablePlugins;

    bool m_addSpectr; //! 0 - создать новый, 1 - выгрузить из библиотеки
    QVector<double> m_XUnits;
    QVector<double> m_YUnits;
    QList<DescriptionSpectr> m_descriptionSpectr;

    bool m_externalSpectr; //! флаг, внешний спектр или спектр с куба
    int m_formatExternalSpectr; // 0 - peleng; 1 - aster

    NoiseAlgorithm m_noiseAlg; //! алгоритм удаления шумов
};

#endif // ATTRIBUTES_H
