#ifndef STRUCTURES
#define STRUCTURES

#include <QString>

struct DescriptionSpectr
{
    QString title;
    QString description;
};

enum Measurements
{
    ECP,         // ЭЦП
    RSD,         // СПЭЯ (radiance spectral density)
    RFL_percent, // Reflectance, % (коэффициент отражения, %)
    RFL_units,   // Reflectance, parts of units (коэффициет отражения в долях единиц)
};


#endif // STRUCTURES

