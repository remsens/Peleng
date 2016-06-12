#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "Types.h"

#include <QList>
#include <QVector>
#include "../Library/structures.h"

//! Структура метаданных для гипекуба
struct InfoData {
    u::uint32 bands; //!< количество каналов
    u::uint32 lines; //!< количество линий
    u::uint32 samples; //!< количество столбцов
    u::uint8 bytesType; //!< количество байт в одном элементе (возможно рудимент)

    u::uint8 formatType; //!< формат данных, согласно Лине
    std::list<double> listChannels; //!<  список длин волн, соответствующих каждому каналу
};

enum TYPES {
    type_int8,
    type_uint8,
    type_int16,
    type_uint16,
    type_int32,
    type_uint32,
    type_int64,
    type_uint64,
    type_float,
    type_double,
    type_2double
};

//! http://www.gdal.org/gdal_datamodel.html
struct BLrad{double breadth,longitude;}; //!< широта и долгота точки в радианах
struct geoData{
    double GeoTransform[6];
    int UTMzone;
    bool northernHemisphere;
    char* GeographCordSys;
    QVector<BLrad> cornerPoints; //!< вектор с координатами 4 угловых точек гиперснимка
};

//! Класс гиперкуба. Хранит данные гиперкуба и метаданные)
class HyperCube {

public:
    //! Конструктор
    HyperCube();
    //! Деструктор
    virtual ~HyperCube();

    //! Функция установки метаданных в гиперкуб. Также происходит выделение памяти под данные
    //! @param infoData - метаданные гиперкуба
    void SetInfoData(const InfoData& infoData);

    //! Функция записи данных.
    //! @param channel - номер канала, в который неоходимо записать данные
    //! @param data - указатель на буффер данных
    //! @param size - размер данных в байтах, который неоходимо записать
    //! @param iteratorBefore - используется при записи в цикле, смещение в байтах для канала, на которые нужно сместиться перед записью данных
    void SetDataBuffer(u::uint32 channel, u::cptr data, u::uint32 size, u::uint32 iteratorBefore = 0);

    //! Функция записи данных для одного канала. !!! Память должна быть уже выделена
    //! @param data - указатель на блок данных
    //! @param channel - номер канала
    void SetDataChannel(u::cptr data, u::uint32 channel);

    //! Функция записи данных для одного спектра в гиперкуб. !!! Память под данные должна быть уже выделена
    //! @param data - указатель на блок данных
    //! @param x - номер строки точки
    //! @param у - номер столбца точки
    void SetDataSpectrum(u::cptr data, u::uint32 x, u::uint32 y);

    //!Функция задания геоматрицы
    //! @param GeoTransform[] - матрица из 6 элементов (http://www.gdal.org/gdal_datamodel.html)
    void SetGeoDataGeoTransform(double GeoTransform[]);

    //!Функция задания UTM зоны
    //! @param zone - номер UTM зоны
    //! @param north - TRUE for northern hemisphere, or FALSE for southern hemisphere.
    void SetGeoDataUTMzone(int zone, bool north);

    //! Функция задания геодезической системы
    //! @param GeographCordSys - одна из стандартных координатных систем. Пример: "NAD27"
    void SetGeoDataGeographCordSys(char* GeographCordSys);

    //! Функция удаления данных из куба. (Под вопросом, нужно ли удалять метаданные?)
    void DestroyCube();

    void UpdateListWaves(QList<double>& wavelength);
    //! Функция получения числа каналов
    //! @return - число каналов гиперкуба
    u::uint32 GetCountofChannels();

    //! Функция получения списка длин волн, соответствующие каждому каналу
    //! @return - список длин волн
    QList<double> GetListOfChannels();

    //! Функция получения количества линий (строк)
    //! @return - количество линий (строк)
	u::uint32 GetLines();
	
    //! Функция получения количества столбцов
    //! @return - количество столбцов
	u::uint32 GetColumns();
	
    //! Функция получения количества байт в одном элементе (возможно рудимент)
    //! @return - количество байт в одном элементе
	u::uint8 GetBytesInElements();

    //! Функция получения типа формата данных, согласно Лине:)
    //! @return - формат данных
    u::uint8 GetFormatType();

    //! Функция получения размера в байтах формата данных
    //! @return - формат данных
    u::uint8 GetSizeOfFormatType();

    //! Функция получения размера гиперкуба в байтах
    //! @return - размер гиперкуба кол-во элементов)
    u::uint32 GetSizeCube() const;


    //! Функция получения данных гиперкуба
    //! @return указатель на данные гиперкуба (двумерный массив, который можно привести к любому типу)
    u::ptr* GetDataCube() const;

    //! Функция получения размера данных спектра одной точки (кол-во элементов)
    //! //! @return - размер данных спектра (кол-во элементов)
    u::uint32 GetSizeSpectrum();

    //! Функция получения данных спектра одной точки
    //! @param x - координата по строкам
    //! @param y - координата по столбцам
    //! @param data - указатель на массив данных, куда будет записан спектр ( под него уже должна быть выделена память!)
	void GetSpectrumPoint(u::uint32 x, u::uint32 y, u::ptr data);

    //! Функция получения данных спектра одной точки
    //! @param x - координата по строкам
    //! @param y - координата по столбцам
    //! @param data - вектор, куда будет записан спектр ( под него уже должна быть выделена память!)
    void GetSpectrumPoint(u::uint32 x, u::uint32 y, QVector<double>& data);

    //! Функция получения размера данных одного канала в байтах
    //! @return - размер данных канала в байтах
	u::uint32 GetSizeChannel();

    //! Функция получения данных одного канала
    //! @param channel - номер канала
    //! @param data - указатель на массив данных, куда будут записаны данные одного канала ( под него уже должна быть выделена память!)
	void GetDataChannel(u::uint32 channel, u::ptr data);

    //! Функция получения данных одного канала
    //! @param channel - номер канала
    //! @param data - вектор, куда будут записаны данные одного канала ( под него уже должна быть выделена память!)
    void GetDataChannel(u::uint32 channel, QVector<double>& data);

    //! @param geo6arr - массив[6], в который будет записана геоматрица
    void GetGeoDataGeoTransform(double (&geo6arr)[6]);
    int GetGeoDataUTMzone(){return m_geoData.UTMzone;}
    bool GetGeoDataUTMzoneNorth(){return m_geoData.northernHemisphere;}
    char* GetGeoDataGeographCordSys(){return m_geoData.GeographCordSys;}

    //! Функция изменения размера гиперкуба (параметры функции - координаты, по которым будет вырезаться куб)
    //! Перед вызовом этой функции надо сохранить куб в hdf!!!
    //! @param Ch1 - начальный номер канала
    //! @param Ch2 - конечный номер канала
    //! @param R1 - начальный номер строк
    //! @param R2 - конечный номер строк
    //! @param C1 - начальный номер столбцов
    //! @param C2 - конечный номер столбцов
    void ResizeCube(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2);

    //! Функция получения  одной точки из куба
    //! @param x - координата по строкам
    //! @param y - координата по столбцам
    //! @param z - координата по каналам
    //! @return - значение по координатам (x,y,z)
    double GetDataPoint(u::uint32 x, u::uint32 y, u::uint32 z);

    void SetMeasurement(Measurements measurement);
    void GetMeasurements(Measurements& measurement);

    QVector<BLrad> getCornerPoints() const;
    void setCornerPoints(const QVector<BLrad> &value);

private:
    u::int8** m_dataCube; //!< двумерный массив данных гиперкуба
    u::uint32 m_sizeCube; //!< размер куба
    InfoData m_infoData; //!< метаданные куба
    Measurements m_measurements; //!< единицы измерения гиперкуба
    geoData m_geoData;
};

#endif
