#ifndef SPECTR_H
#define SPECTR_H

#include "../Library/HyperCube.h"
#include <QVector>
#include "Types.h"
#include "structures.h"


/*!
 * Класс-хранилище для спектральных кривых
 * Содержить все информацию о собственном или загруженном спектре
*/

class Spectr
{
public:
    /*!
      *Конструктор по умолчанию
    */
    Spectr();

    /*!
     * Конструктор
     \param[in] x Координата X в гиперкубе
     \param[in] y Координата Y в гиперкубе
    */
    Spectr(HyperCube* cube, u::uint32 x, u::uint32 y);

    /*!
     * Конструктор
     \param[in] xUnits Значения по х координате
     \param[in] yUnits Значения по у координате
     \param[in] title  Название спектра
     \param[in] measurements Единицы измерения
     \param[in] spectrDescription Полное описание спектра
    */
    Spectr(HyperCube* cube, const QVector<double>& xUnits, const QVector<double>& yUnits, QString& title, Measurements measurements, const QList<DescriptionSpectr>& spectrDescription);

    /*!
     * Конструктор
     \param[in] xUnits          Значения по х координате
     \param[in] yUnits          Значения по у координате
     \param[in] title           Название спектра
     \param[in] measurements    Единицы измерения
    */
    Spectr(HyperCube* cube, const QVector<double>& xUnits, const QVector<double>& yUnits, QString& title, Measurements measurements);

    /*!
     * Деструктор
     */
    virtual ~Spectr();

    QVector<double> GetXUnits() const;
    QVector<double> GetYUnits() const;
    QVector<double> GetXUnitsInterpolated() const;
    QVector<double> GetYUnitsInterpolated() const;
    QVector<double> GetXUnitsNormed() const;
    QVector<double> GetYUnitsNormed() const;
    QString GetTitle() const;
    u::uint32 GetXCoord() const;
    u::uint32 GetYCoord() const;
    Measurements GetMeasurements() const;
    QList<DescriptionSpectr> GetDescriptionOfSpectr() const;
    void SetDescriptionItem(QString& title, QString& description);
    void SetInterpolated();
    bool GetInterpolated();

    void SetNormed();
    bool GetNormed();

private:
    void Interpolate();
    void Norm();

private:
    HyperCube* m_cube;                              ///< Указатель на объект класса гиперкуб
    QVector<double> m_xUnits;                       ///< Значения по х координате
    QVector<double> m_yUnits;                       ///< Значения по у координате

    QVector<double> m_xUnitsInterpolated;           ///< Интерполированные значения по х координате
    QVector<double> m_yUnitsInterpolated;           ///< Интерполированные значения по у координате

    QVector<double> m_xUnitsNormed;                 ///< Нормированные значения по х координате
    QVector<double> m_yUnitsNormed;                 ///< Нормированные значения по у координате

    QString   m_title;                              ///< Название спектра
    u::uint32 m_xCoord;                             ///< Координата X в гиперкубе
    u::uint32 m_yCoord;                             ///< Координата Y в гиперкубе
    Measurements m_measurements;                    ///< Единицы измерения
    QList<DescriptionSpectr> m_spectrDescription;   ///< Полное описание спектра
    bool m_interpolated;                            ///< флаг, есть ли нормированные данные
    bool m_normed;                                  ///< флаг, нормированные данные или нет
};

#endif // SPECTR_H
