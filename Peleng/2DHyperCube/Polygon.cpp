#include "Polygon.h"

Polygon::Polygon()
{

}

Polygon::~Polygon()
{

}

void Polygon::addPoint(double x, double y)
{
    m_points.append(Point(x,y));
}

void Polygon::addPoint(Point p)
{
    m_points.append(p);
}

bool Polygon::isInsidePolygon(Point p)
{
    //https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Задача_о_принадлежности_точки_многоугольнику
    return false;
}

