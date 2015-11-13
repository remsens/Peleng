#ifndef POLYGON_H
#define POLYGON_H
#include <qvector.h>

struct Point{
    double x; // или uint
    double y;
    Point() : x(0),y(0) {}
    Point(double x,double y) : x(x),y(y) {}
};

class Polygon
{
public:
    Polygon();
    ~Polygon();
    //! перегруженная функция добавления вершины полигона
    //! @param x - x координата
    //! @param y - y координата
    void addPoint(double x, double y);
    //! перегруженная функция добавления вершины полигона
    //! @param p - точка
    void addPoint(Point p);
    //! функция проверки находится ли точка внутри полигона
    //! @param p - точка
    //! @return - внутри полигона или нет
    bool isInsidePolygon(Point p);
private:
    QVector<Point> m_points;

};

#endif // POLYGON_H
