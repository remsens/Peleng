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

    void SetPointsList(const Point &point);
    void SetPoint(u::uint32 x, u::uint32 y, u::uint32 z);
    void ClearList();
    void SetAvailablePlugins(const QMap<QString, ProcessingPluginsInterface*> availablePlugins);

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
};

#endif // ATTRIBUTES_H
