#include "Spectr.h"

#include "../Library/MathOperations/interpolation.h"

Spectr::Spectr()
{

}

Spectr::Spectr(HyperCube* cube, u::uint32 x, u::uint32 y)
{
    m_xCoord = x;
    m_yCoord = y;
    m_interpolated = false;
    m_normed = false;
    m_cube = cube;
    for (int i = 0; i < m_cube->GetCountofChannels(); i++)
    {
        m_xUnits.append(m_cube->GetListOfChannels().at(i));
    }
    m_cube->GetSpectrumPoint(x, y, m_yUnits);
}

Spectr::Spectr(HyperCube* cube, const QVector<double>& xUnits, const QVector<double>& yUnits, QString& title, Measurements measurements, const QList<DescriptionSpectr>& spectrDescription)
{
    m_xUnits = xUnits;
    m_yUnits = yUnits;
    m_title = title;
    m_measurements = measurements;
    m_spectrDescription = spectrDescription;
    m_interpolated = false;
    m_normed = false;
    m_cube = cube;
}

Spectr::Spectr(HyperCube* cube, const QVector<double>& xUnits, const QVector<double>& yUnits, QString& title, Measurements measurements)
{
    m_xUnits = xUnits;
    m_yUnits = yUnits;
    m_title = title;
    m_measurements = measurements;
    m_interpolated = false;
    m_normed = false;
    m_cube = cube;
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
    m_title.clear();
}

void Spectr::Interpolate()
{
    interpolate(m_xUnits, m_yUnits, m_cube->GetListOfChannels(),m_yUnitsInterpolated);
    for (u::uint32 i = 0; i < m_cube->GetCountofChannels(); i++)
    {
        m_xUnits.push_back(m_cube->GetListOfChannels().at(i));
    }
    m_interpolated = true;
}

void Spectr::Norm()
{
    QVector<double> sortedYArr;
    sortedYArr = m_yUnits;
    qSort(sortedYArr);
    m_xUnitsNormed = m_xUnits;
    for (int i = 0; i < m_yUnits.size(); i++)
    {
        m_yUnitsNormed.push_back(m_yUnits.at(i)/sortedYArr.last());
    }
    m_normed = true;
}

QVector<double> Spectr::GetXUnits() const
{
    return m_xUnits;
}
QVector<double> Spectr::GetYUnits() const
{
    return m_yUnits;
}

QVector<double> Spectr::GetXUnitsInterpolated() const
{
    return m_xUnitsInterpolated;
}
QVector<double> Spectr::GetYUnitsInterpolated() const
{
    return m_yUnitsInterpolated;
}
QVector<double> Spectr::GetXUnitsNormed() const
{
    return m_xUnitsNormed;
}
QVector<double> Spectr::GetYUnitsNormed() const
{
    return m_yUnitsNormed;
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
Measurements Spectr::GetMeasurements() const
{
    return m_measurements;
}
QList<DescriptionSpectr> Spectr::GetDescriptionOfSpectr() const
{
    return m_spectrDescription;
}

void Spectr::SetInterpolated()
{
    m_interpolated = true;
}
bool Spectr::GetInterpolated()
{
    return m_interpolated;
}

void Spectr::SetNormed()
{
    m_normed = true;
}
bool Spectr::GetNormed()
{
    return m_normed;
}

void Spectr::SetDescriptionItem(QString& title, QString& description)
{
    DescriptionSpectr ds;
    ds.title = title;
    ds.description = description;
    m_spectrDescription.append(ds);
}
