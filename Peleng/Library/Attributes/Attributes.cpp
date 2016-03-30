#include "Attributes.h"

Attributes* Attributes::m_instance = NULL;

Attributes::Attributes()
{

}
Attributes* Attributes::I()
{
    if (!m_instance)
    {
        m_instance = new Attributes;
    }
    return m_instance;
}

 void Attributes::Destroy()
 {
     if (m_instance)
     {
         delete m_instance;
     }
 }

Attributes::~Attributes()
{

}

void Attributes::SetModeLib(bool value)
{
    m_addSpectr = value;
}

bool Attributes::GetModeLib() const
{
    return m_addSpectr;
}

void Attributes::SetXUnit(double xUnit)
{
    m_XUnits.push_back(xUnit);
}

void Attributes::SetYUnit(double yUnit)
{
    m_YUnits.push_back(yUnit);
}

void Attributes::SetXUnit(QVector<double>& xUnits)
{
    m_XUnits.clear();
//    m_XUnits.append(xUnits);
    foreach (double x, xUnits) {
        m_XUnits.append(x);
    }
}

void Attributes::SetYUnit(QVector<double>& yUnits)
{
    m_YUnits.clear();
    //m_YUnits.append(yUnits);
    foreach (double y, yUnits) {
        m_YUnits.append(y);
    }
}

void Attributes::ClearUnitsLists()
{
    m_XUnits.clear();
    m_YUnits.clear();
    m_descriptionSpectr.clear();
}

void Attributes::SetDescriptionItem(const QString& keyTitle, const QString& value)
{
    DescriptionSpectr ds;
    ds.title = keyTitle;
    ds.description = value;
    m_descriptionSpectr.append(ds);
}

void Attributes::SetDescriptionSpectr(QList<DescriptionSpectr>& ds)
{
    m_descriptionSpectr.clear();
    m_descriptionSpectr.append(ds);
}

const QVector<double>& Attributes::GetXUnits() const
{
    return m_XUnits;
}

const QVector<double>& Attributes::GetYUnits() const
{
    return m_YUnits;
}

const QList<Attributes::DescriptionSpectr> &Attributes::GetSpectrumDescription() const
{
    return m_descriptionSpectr;
}

void Attributes::SetPointsList(const Point& point)
{
    m_pointsList.append(point);
}

void Attributes::ClearList() {
    m_pointsList.clear();
}

void Attributes::SetPoint(u::uint32 x, u::uint32 y, u::uint32 z)
{
    Point point;
    point.x = x; point.y = y; point.z = z;
    SetPointsList(point);
}

void Attributes::SetAvailablePlugins(const QMap<QString, ProcessingPluginsInterface*> availablePlugins)
{
    m_availablePlugins.clear();
    m_availablePlugins = availablePlugins;
}

const QList<Point>& Attributes::GetPointsList() const {
    return m_pointsList;
}


const QMap<QString, ProcessingPluginsInterface *> &Attributes::GetAvailablePlugins() const
{
    return m_availablePlugins;
}

void Attributes::SetExternalSpectrFlag(bool externalSpectr)
{
    m_externalSpectr = externalSpectr;
}

bool Attributes::GetExternalSpectrFlag() const
{
    return m_externalSpectr;
}

void Attributes::SetSPlotterYtitle(QString Yunits)
{
    m_SPlotter_Ytitle = Yunits;
}

QString Attributes::GetSPlotterYtitle() const
{
    return m_SPlotter_Ytitle;
}

//int  Attributes::GetFormatExternalSpectr() const
//{
//    return m_formatExternalSpectr;
//}

//void Attributes::SetExternalSpectrFormat(int format)
//{
//    m_formatExternalSpectr = format;
//}

void Attributes::SetNoiseAlg(NoiseAlgorithm alg)
{
    m_noiseAlg = alg;
}

NoiseAlgorithm Attributes::GetNoiseAlg() const
{
    return m_noiseAlg;
}

void Attributes::SetMaskPixelsCount(const u::uint32 pixelsNumber)
{
    m_maskPixelsCount = pixelsNumber;
}

u::uint32 Attributes::GetMaskPixelsCount() const
{
    return m_maskPixelsCount;
}

void Attributes::SetApplyToAllCube(bool apply)
{
    m_applyToAllCube = apply;
}

bool Attributes::GetApplyToAllCube() const
{
    return m_applyToAllCube;
}

void Attributes::SetDegreePolinom(u::uint8 degree)
{
    m_degreePolinom = degree;
}

u::uint8 Attributes::GetDegreePolinom() const
{
    return m_degreePolinom;
}

void Attributes::SetTempChanel(double* pChan)
{
    m_tempChan = pChan;
}

double* Attributes::GetTempChanel()
{
    return m_tempChan;
}
