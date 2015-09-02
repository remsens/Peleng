#ifndef GETHYPERCUBE_H
#define GETHYPERCUBE_H
#include "HyperCube.h"
#include <QObject>
class GetHyperCube
{
public:
    GetHyperCube();
    HyperCube* GetCube() const;
    ~GetHyperCube();
private:
    qint16** data;
    void CreateHyperCube();
    HyperCube* m_hyperCube;
};

#endif // GETHYPERCUBE_H
