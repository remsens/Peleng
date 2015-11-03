#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H


#include <QObject>
#include <QMap>
#include <QString>
#include <QList>
#include <../Library/PointStruct.h>


class Attributes;
#include "../Library/Interfaces/ProcessingPluginInterface.h"


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
    void ClearUnitsLists();
    void SetDescriptionIten(const QString& keyTitle, const QString& value);

    const QVector<double>& GetXUnits() const;
    const QVector<double>& GetYUnits() const;
    const QMap<QString, QString>& GetSpectrumDescription() const;
    // общее
    const QList<Point>& GetPointsList() const;
    const QMap<QString, ProcessingPluginsInterface*>& GetAvailablePlugins() const;

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
    QMap<QString, QString> m_descriptionSpectr;
};

#endif // ATTRIBUTES_H
