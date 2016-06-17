#include "Spectr.h"

#include "../Library/MathOperations/interpolation.h"
#include <QDebug>

Spectr::Spectr()
{

}

Spectr::Spectr(HyperCube* cube, u::uint32 x, u::uint32 y)
{
    m_xCoord = x;
    m_yCoord = y;
    m_cube = cube;
    for (uint i = 0; i < m_cube->GetCountofChannels(); i++)
    {
        m_xUnits.append(m_cube->GetListOfChannels().at(i));
    }
    m_currentDataType = NONPROCESSING;
    m_cube->GetSpectrumPoint(x, y, m_yUnits);
    // нормировка простых данных
    Norm(m_xUnits, m_yUnits, m_xUnitsNormed, m_yUnitsNormed);
    // интерполяция простых данных
    Interpolate(m_xUnits, m_yUnits, m_xUnitsInterpolated, m_yUnitsInterpolated);
    // нормировка интерполированных данных
    Norm(m_xUnitsInterpolated, m_yUnitsInterpolated, m_xUnitsInterpolatedNormed, m_yUnitsInterpolatedNormed);
    // интерполяция нормированных данных
    Interpolate(m_xUnitsNormed, m_yUnitsNormed, m_xUnitsNormedInterpolated, m_yUnitsNormedInterpolated);
    m_title = "X = " + QString::number(x) + " Y = " + QString::number(y);    
    m_cube->GetMeasurements(m_measurements);
    //qDebug() << "Единицы измерения куба" + QString::number(m_measurements);
}

Spectr::Spectr(HyperCube* cube, const QVector<double>& xUnits, const QVector<double>& yUnits, QString& title, Measurements measurements, const QList<DescriptionSpectr>& spectrDescription)
{
    m_xUnits = xUnits;
    m_yUnits = yUnits;
    m_title = title;
    m_measurements = measurements;
    m_spectrDescription = spectrDescription;
    m_cube = cube;
    m_currentDataType = NONPROCESSING;
    // нормировка простых данных
    Norm(m_xUnits, m_yUnits, m_xUnitsNormed, m_yUnitsNormed);
    // интерполяция простых данных
    Interpolate(m_xUnits, m_yUnits, m_xUnitsInterpolated, m_yUnitsInterpolated);
    // нормировка интерполированных данных
    Norm(m_xUnitsInterpolated, m_yUnitsInterpolated, m_xUnitsInterpolatedNormed, m_yUnitsInterpolatedNormed);
    // интерполяция нормированных данных
    Interpolate(m_xUnitsNormed, m_yUnitsNormed, m_xUnitsNormedInterpolated, m_yUnitsNormedInterpolated);

}

Spectr::Spectr(HyperCube* cube, const QVector<double>& xUnits, const QVector<double>& yUnits, QString& title, Measurements measurements)
{
    m_xUnits = xUnits;
    m_yUnits = yUnits;
    m_title = title;
    m_measurements = measurements;
    m_cube = cube;
    m_currentDataType = NONPROCESSING;
    // нормировка простых данных
    Norm(m_xUnits, m_yUnits, m_xUnitsNormed, m_yUnitsNormed);
    // интерполяция простых данных
    Interpolate(m_xUnits, m_yUnits, m_xUnitsInterpolated, m_yUnitsInterpolated);
    // нормировка интерполированных данных
    Norm(m_xUnitsInterpolated, m_yUnitsInterpolated, m_xUnitsInterpolatedNormed, m_yUnitsInterpolatedNormed);
    // интерполяция нормированных данных
    Interpolate(m_xUnitsNormed, m_yUnitsNormed, m_xUnitsNormedInterpolated, m_yUnitsNormedInterpolated);

}

Spectr::~Spectr()
{
    m_spectrDescription.clear();
    m_xUnits.clear();
    m_yUnits.clear();
    m_xUnitsInterpolated.clear();
    m_yUnitsInterpolated.clear();
    m_xUnitsNormed.clear();
    m_yUnitsNormed.clear();
    m_xUnitsInterpolatedNormed.clear();
    m_yUnitsInterpolatedNormed.clear();
    m_xUnitsNormedInterpolated.clear();
    m_xUnitsNormedInterpolated.clear();
    m_title.clear();
}

void Spectr::SetMeasurement(Measurements measurement)
{
    m_measurements = measurement;
}

QVector<double> Spectr::GetCurrentDataX() const
{
    switch (m_currentDataType)
    {
        case NONPROCESSING: return m_xUnits;
        case NORMED: return m_xUnitsNormed;
        case INTERPOLATE: return m_xUnitsInterpolated;
        case NORMED_INTERPOLATE: return m_xUnitsNormedInterpolated;
        case INTERPOLATE_NORMED: return m_xUnitsInterpolatedNormed;
    }
}

QVector<double> Spectr::GetCurrentDataY() const
{
    switch (m_currentDataType)
    {
        case NONPROCESSING: return m_yUnits;
        case NORMED: return m_yUnitsNormed;
        case INTERPOLATE: return m_yUnitsInterpolated;
        case NORMED_INTERPOLATE: return m_yUnitsNormedInterpolated;
        case INTERPOLATE_NORMED: return m_yUnitsInterpolatedNormed;
    }
}

void Spectr::SetCurrentDataType(CurrentDataType currentDataType)
{
    m_currentDataType = currentDataType;
}

Spectr::CurrentDataType Spectr::GetCurrentDataType() const
{
    return m_currentDataType;
}

void Spectr::Interpolate(QVector<double>& xUnitsOld, QVector<double>& yUnitsOld, QVector<double>& xUnitsNew, QVector<double>& yUnitsNew)
{
    for (u::uint32 i = 0; i < m_cube->GetCountofChannels(); i++)
    {
        xUnitsNew.push_back(m_cube->GetListOfChannels().at(i));
    }
    yUnitsNew.resize(m_cube->GetCountofChannels());
    interpolate(xUnitsOld, yUnitsOld, xUnitsNew, yUnitsNew);
}

void Spectr::Norm(QVector<double> &xUnitsOld, QVector<double> &yUnitsOld, QVector<double> &xUnitsNew, QVector<double> &yUnitsNew)
{
    QVector<double> sortedYArr;
    sortedYArr = yUnitsOld;
    qSort(sortedYArr);
    xUnitsNew = xUnitsOld;
    for (int i = 0; i < yUnitsOld.size(); i++)
    {
        yUnitsNew.push_back(yUnitsOld.at(i)/sortedYArr.last());
    }
}

QString Spectr::GetTitle() const
{
    return m_title;
}
u::uint32 Spectr::GetXCoord() const
{
    return m_xCoord;
}
u::uint32 Spectr::GetYCoord() const
{
    return m_yCoord;
}

void Spectr::GetMeasurements(Measurements &measurement)
{
    qDebug() << "Единицы измерения спектра" + QString::number(m_measurements);
    measurement = m_measurements;
}

QList<DescriptionSpectr> Spectr::GetDescriptionOfSpectr() const
{
    return m_spectrDescription;
}

void Spectr::SetDescriptionItem(QString& title, QString& description)
{
    DescriptionSpectr ds;
    ds.title = title;
    ds.description = description;
    m_spectrDescription.append(ds);
}
