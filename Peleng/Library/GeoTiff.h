#ifndef GEOTIFF_H
#define GEOTIFF_H

#include <QObject>
#include "HyperCube.h"

class GeoTiff
{
public:
    GeoTiff();
    static bool save(const char* dstName, HyperCube* cube);// еще передать параметры: время, координаты и т.п.
};

#endif // GEOTIFF_H
