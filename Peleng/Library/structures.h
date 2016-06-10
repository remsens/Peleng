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
    Relative_units, // Относительные единицы
    Unknown_measurement      // Неизвестный тип
};

inline QString GetTextMeasurements(Measurements measurement)
{
    switch (measurement)
    {
        case Measurements::ADC: return QString("Яркость, единицы АЦП");
        case Measurements::RFL_percent: return QString("Коэффициент отражения, %");
        case Measurements::RFL_units: return QString("Коэффициент отражения, доли единиц");
        case Measurements::RSD: return QString("СПЭЯ, Вт/(см2*мкм*ср)");
        case Measurements::Relative_units: return QString("Относительные единицы");
        default: return QString("Неизвеcтные величины измерения");
    }
}
#endif // STRUCTURES

