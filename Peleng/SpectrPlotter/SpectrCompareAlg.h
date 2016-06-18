#ifndef SPECTRCOMPAREALG_H
#define SPECTRCOMPAREALG_H

#include <QVector>

class SpectrCompareAlg
{
public:
    SpectrCompareAlg();
    virtual ~SpectrCompareAlg();
    static double EvclidDistance(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end);
    static double SpectralDistance(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end);
    static double SpectralAngle(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end);
    static double SpectralCorrelation(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end);
    static double SID(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end);
    static double Entropy(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end);
};

#endif // SPECTRCOMPAREALG_H
