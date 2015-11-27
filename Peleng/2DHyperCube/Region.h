#ifndef REGION_H
#define REGION_H
#include <QObject>
#include <QColor>
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
};

#endif // REGION_H
