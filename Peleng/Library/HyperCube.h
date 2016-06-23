#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "Types.h"

#include <QList>
#include <QVector>
#include "../Library/structures.h"
#include "../Library/Ellipsoid.h"

//! Структура метаданных для гипекуба
struct InfoData {
    u::uint32 bands; //!< количество каналов
    u::uint32 lines; //!< количество линий
    u::uint32 samples; //!< количество столбцов
    u::uint8 bytesType; //!< количество байт в одном элементе (возможно рудимент)

    u::uint8 formatType; //!< формат данных, согласно Лине
    std::list<double> listChannels; //!<  список длин волн, соответствующих каждому каналу
};

struct Date {
    u::uint32 day;
    u::uint32 month;
    u::uint32 year;
};

struct Time {
    u::uint32 hours;
    u::uint32 minutes;
    u::uint32 seconds;
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
    type_2double,
    type_unknown
};

//! http://www.gdal.org/gdal_datamodel.html
struct BLrad
{
    double breadth,longitude;
    BLrad(double bl=0, double lon=0):breadth(bl),longitude(lon){}

}; //!< широта и долгота точки в радианах
struct  point { double   x,y; };
struct  pointInt { int   x,y; };
struct geoData{
    double GeoTransform[6];
    bool northernHemisphere;
    char* GeographCordSys;
    QVector<BLrad> cornerPoints;    //!< вектор с координатами (в радианах) 4-х угловых точек гиперснимка.
                                    //!< Обход по часовой стрелке начиная с (0,0) вершины
    TEllipsoid earth;
    double rotationAngle;           //!< угол поворота снимка в радианах от 0 до 2Pi
    double pixelSizeX;              //!< размер пикселя в метрах по оси Х (т.е. ширина 1 sampl'a)
    double pixelSizeY;              //!< размер пикселя в метрах по оси Y (т.е. высота 1 line'a)
    point point00;                  //!< UTM координаты точки куба c координатами sample = 0, line = 0
    char utmZone[4];                //! зона UTM в формате для TEllipsoid

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

    //! Функция полушария (нужна для сохранения в ENVI)
    //! @param north - TRUE for northern hemisphere, or FALSE for southern hemisphere.
    void SetNorthernHemisphere (bool north);

    //! Функция задания геодезической системы
    //! @param GeographCordSys - одна из стандартных координатных систем. Пример: "WGS84"
    void SetGeoDataGeographCordSys(char* GeographCordSys);

    void SetDate(const Date& date);
    void SetTime(const Time& time);
    void SetAltitude(u::uint32 altitude);
    void SetViewingDirection(u::uint32 viewingDirection);
    void SetAperture(u::uint32 aperture);
   // void SetRotationAngle(u::uint32 rotationAngle);

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

    //! Функция получения геоматрицы (для сохранения в ENVI)
    //! @param geo6arr - массив[6], в который будет записана геоматрица
    void GetGeoDataGeoTransform(double (&geo6arr)[6]);

    //! Функция получения полушария (для сохранения в ENVI)
    bool GetNorthernHemisphere(){return m_geoData.northernHemisphere;}

     //! Функция получения географической координатной системы
    char* GetGeoDataGeographCordSys(){return m_geoData.GeographCordSys;}

    Date GetDate();
    Time GetTime();
    u::uint32 GetAltitude();
    u::uint32 GetViewingDirection();
    u::uint32 GetAperture();
    u::uint32 GetRotationAngle();

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

    //! функция инициализации элипсоида Земли
    //! @param iell - номер модели элипсоида
    void initElipsoid(long iell);

    //! возвращает элипсоид
    TEllipsoid getElipsoid() const;

    //! get/set угла поворота изображения в радианах от 0 до 2 Pi
    double getRotationAngle() const;
    void setRotationAngle(double value);

    //! Функция возвращает координату в UTM зоне для любой точки гиперкуба
    point getUTMcords(int row, int col);

    //! Функция возвращает широту/долготу в градусах для любой точки гиперкуба
    point getBLdegreeCords(int row, int col);

    //! Функция возвращает номер строки и столбца гиперкуба для любой заданной UTM координаты
    //! возвращаемые значения могут выходить за границы допустимых индексов гиперкуба
    pointInt getImageCordsFromUTM(double utmX, double utmY);

    //! Функция возвращает номер строки и столбца гиперкуба для любой заданной широты/долготы в град.
    //! возвращаемые значения могут выходить за границы допустимых индексов гиперкуба
    pointInt getImageCordsFromBLdeg(double breadthDeg, double longitudeDeg);

    //! get/set размера пикселя по Х в метрах
    double getPixelSizeX() const;
    void setPixelSizeX(double value);

    //! get/set размера пикселя по Х в метрах
    double getPixelSizeY() const;
    void setPixelSizeY(double value);

    //! get/set  UTM координат точки куба с координатами sample = 0, line = 0
    point getPoint00() const;
    void setPoint00(const point &value);
    void setPoint00(double x, double y);

    //! get/set  UTM зоны, в которой находится гиперкуб
    void setUTMforElipsoid(char zone[]);
    void getUTMforElipsoid(char *outZone);


private:
    u::int8** m_dataCube; //!< двумерный массив данных гиперкуба
    u::uint32 m_sizeCube; //!< размер куба
    InfoData m_infoData; //!< метаданные куба
    Measurements m_measurements; //!< единицы измерения гиперкуба
    geoData m_geoData;
    Date m_date;
    Time m_time;
    u::uint32 m_viewingDirection;
    u::uint32 m_altitude;
    u::uint32 m_aperture;
    u::uint32 m_rotationAngle;
};

#endif
