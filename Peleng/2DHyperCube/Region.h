#ifndef REGION_H
#define REGION_H
#include <QObject>
#include <QColor>
#include <QVector>
#include <QPolygonF>
#include "../Library/QCustomPlot.h"


struct PolygonObject
{
    QPolygonF ijVertices;            //!< полигон, в вершинах которого пиксельные координаты
    QPolygonF BLdegVertices;        //!< полигон, в вершинах которого широта/долгота в десятичных градусах
    QVector<QCPItemLine *> lines; //!< линии соединяющие вершины
};

class Region
{
   // Q_OBJECT
public:
    Region();
    ~Region();

public:
    QByteArray m_byteArr; //удалить
    QString m_name;                         //!< название региона
    QColor m_color;                         //!< цвет региона
    QVector<PolygonObject> m_polygonObjects;      //!< полигоны из которых состоит регион
    QCPItemPixmap *m_pixItem;               //!< указатель на полупрозрачную картинку с регионом
                                            //!  (и линии стираются с customplota) при его завершении
};

#endif // REGION_H
