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
private:
    bool parseHeaderFile(QString& headerFilePath);
    bool getDataFilePath(const QString& headerFilePath, QString& dataFilePath);
    bool setMetaDataToCube(HyperCube* cube);
    bool readDataToCube(HyperCube* cube, const QString &fileName);
private:
    enum Interleave {BSQ, BIL, BIP};

    u::uint32 TypeFromAvirisType(u::int32 format);
    u::uint32 GetNumberOfBytesFromData(u::int32 format);
    u::logic ReadBSQ(const QString& fileName, HyperCube* cube);
    u::logic ReadBIL(const QString& fileName, HyperCube* cube);
    u::logic ReadBIP(const QString& fileName, HyperCube* cube);
    void ConvertToLittleEndian(HyperCube* cube);
    void SortByWavelength(HyperCube* cube);
    void calcAngle(HyperCube* cube);

private:
    InfoData m_infoData;
    u::uint8 m_headerOffset;
    u::uint8 m_byteOrder;
    Interleave m_interleave;
    bool m_cancel;
    int m_progress;
};

#endif // CREATERHYPERCUBES_H
