#ifndef CREATERHYPERCUBES_H
#define CREATERHYPERCUBES_H

#include "../Library/HyperCube.h"

class CreaterHyperCubes
{
public:
    CreaterHyperCubes();
    virtual ~CreaterHyperCubes();

    bool CreateCube(QString& headerFilePath, HyperCube* cube);
    void SetCancel();
    int GetProgress();
    QString GetErrorDescription();
private:
    bool parseHeaderFile(QString& headerFilePath);
    bool setMetaDataToCube(HyperCube* cube);
    bool readDataToCube(HyperCube* cube, const QString &fileName);
private:
    enum Interleave {BSQ, BIL, BIP};

    u::uint32 GetNumberOfBytesFromData(u::int32 format);
    u::logic ReadBSQ(const QString& fileName, HyperCube* cube);
    u::logic ReadBIL(const QString& fileName, HyperCube* cube);
    u::logic ReadBIP(const QString& fileName, HyperCube* cube);
    void ConvertToLittleEndian(HyperCube* cube);
    void SortByWavelength(HyperCube* cube);
    //! расчет угла поворота
    double calcRotAngle(HyperCube* cube);

private:
    InfoData m_infoData;
    u::uint8 m_headerOffset;
    u::uint8 m_byteOrder;
    Interleave m_interleave;
    QString m_dataPath;
    QString m_calibrationPath;
    QString m_wavelengthPath;
    QList<double> m_wavelength;
    QList<double> m_calibrationCoeff;
    Date m_date;
    Time m_time;
    TYPES m_type;
    double m_pixelSize;
    u::uint32 m_altitude;
    u::uint32 m_viewingDirection;
    u::uint32 m_aperture;
    u::uint32 m_rotationAngle;
    QVector<BLrad> m_corners;
    bool m_readType;
    bool m_readCalibCoeff;
    bool m_readWaveLenght;
    bool m_cancel;
    int m_progress;
    QString m_errDescription;
};

#endif // CREATERHYPERCUBES_H
