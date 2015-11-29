#ifndef REGION_H
#define REGION_H
#include <QObject>
#include <QColor>
#include <QVector>
#include <QPolygon>
#include "../Library/QCustomPlot.h"
class Region
{
   // Q_OBJECT
public:
    Region();
    ~Region();

public:
    QString m_name;
    QByteArray m_byteArr;
    QColor m_color;
    QVector<QPolygon> m_polygonArr;
    QCPItemPixmap *m_pixItem;
    QVector<QCPItemLine *> m_lines; //вектор с линиями для одного полигона, который обнуляется(и линии стираются с customplota) при его завершении

};

#endif // REGION_H
