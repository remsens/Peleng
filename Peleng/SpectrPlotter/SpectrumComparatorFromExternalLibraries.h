#ifndef SPECTRUMCOMPARATORFROMEXTERNALLIBRARIES_H
#define SPECTRUMCOMPARATORFROMEXTERNALLIBRARIES_H

#include <QMap>
#include "../Library/Types.h"
#include "../Library/Spectr.h"

class SpectrumComparatorFromExternalLibraries
{

public:
    SpectrumComparatorFromExternalLibraries();
    virtual ~SpectrumComparatorFromExternalLibraries();
    enum CompareMode
    {
        EvclidDist, SpectrumAngle, SpectralCorrelation, SpectrumDist, SID, Entropy
    };
    void GetSimilarSpectra(HyperCube *cube, Spectr* currentSpectr, int startWave, int endWave, QString startDir, u::uint32 countOfSimilarSpectra, CompareMode mode, QMap<double, Spectr *> &similarSpectra);

private:
    QStringList getDirFiles( const QString& dirName);
};

#endif // SPECTRUMCOMPARATORFROMEXTERNALLIBRARIES_H
