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
    ADC,         // АЦП
    RSD,         // СПЭЯ (radiance spectral density)
    RFL_percent, // Reflectance, % (коэффициент отражения, %)
    RFL_units,   // Reflectance, parts of units (коэффициет отражения в долях единиц)
};

inline QString GetTextMeasurements(Measurements measurement)
{
    switch (measurement)
    {
        case Measurements::ADC: return QString("Яркость, единицы АЦП");
        case Measurements::RFL_percent: return QString("Коэффициент отражения, %");
        case Measurements::RFL_units: return QString("Коэффициент отражения, доли единиц");
        case Measurements::RSD: return QString("СПЭЯ, Вт/(см2*мкм*ср)");
        default: return QString("Неизвеcтные величины измерения");
    }
}
#endif // STRUCTURES

