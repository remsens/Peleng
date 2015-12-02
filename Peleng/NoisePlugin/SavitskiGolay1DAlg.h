#ifndef SAVITSKIGOLAY1DALG
#define SAVITSKIGOLAY1DALG

#include "BaseNoiseAlg.h"
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include "../Library/Types.h"
#include "../Library/Utils/Compare.h"

template <typename T>
class SavitskiGolay1DAlg : public BaseNoiseAlg<T>
{
public:
    SavitskiGolay1DAlg(HyperCube* cube, Attributes* attr)
        :BaseNoiseAlg<T>(cube, attr)
    {

    }
    virtual ~SavitskiGolay1DAlg()
    {

    }

public:
    virtual bool Execute()
    {

        if (BaseNoiseAlg<T>::m_attributes->GetApplyToAllCube())
        {
            return ToCube();
        } else
        {
            ToSpectrWindow();
            return false;
        }
    }
private:
    // коэффициенты для полинома x^2 и x^3, окно - 5 пикселей. Последнее значение - значение нормировки
    u::int8 coeff_quadr_cubic_5[6] = {-3, 12, 17, 12, -3, 35};
    // коэффициенты для полинома x^2 и x^3, окно - 7 пикселей. Последнее значение - значение нормировки
    u::int8 coeff_quadr_cubic_7[8] = {5, 0, -3, -4, -3, 0, 5, 42};
    // коэффициенты для полинома x^2 и x^3, окно - 9 пикселей. Последнее значение - значение нормировки
    u::int8 coeff_quadr_cubic_9[10] = {28, 7, -8, -17, -20, -17, -8, 7, 28, 462};
    // коэффициенты для полинома x^4 и x^5, окно - 5 пикселей. Последнее значение - значение нормировки
    u::int8 coeff_quartic_quintic_5[6] = {-1, 16, -30, 16, -1, 12};
    // коэффициенты для полинома x^4 и x^5, окно - 7 пикселей. Последнее значение - значение нормировки
    u::int8 coeff_quartic_quintic_7[8] = {-13, 67, -19, -70, -19, 67, -13, 132};
    // коэффициенты для полинома x^4 и x^5, окно - 9 пикселей. Последнее значение - значение нормировки
    u::int8 coeff_quartic_quintic_9[10] = {-126, 371, 151, -211, -370, -211, -151, 371, -126, 1716};

private:

    void ChooseCoef(u::uint8* coeff, u::uint8& normalization)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        switch (maskPixels)
        {
            case 5: {
                switch(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom())
                case 2:
                case 3: { memcpy(coeff, coeff_quadr_cubic_5,  maskPixels); normalization = coeff_quadr_cubic_5[maskPixels]; break; }
                case 4:
                case 5:{ memcpy(coeff, coeff_quartic_quintic_5,  maskPixels); normalization = coeff_quadr_cubic_5[maskPixels]; break; }
                default:
                break;
            }
            case 7:
            {
                switch(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom())
                case 2:
                case 3: { memcpy(coeff, coeff_quadr_cubic_7,  maskPixels); normalization = coeff_quadr_cubic_7[maskPixels]; break; }
                case 4:
                case 5:{ memcpy(coeff, coeff_quartic_quintic_7,  maskPixels); normalization = coeff_quadr_cubic_7[maskPixels]; break; }
                default:
                break;
            }
            case 9:
            {
                switch(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom())
                case 2:
                case 3: { memcpy(coeff, coeff_quadr_cubic_9,  maskPixels); normalization = coeff_quadr_cubic_9[maskPixels]; break; }
                case 4:
                case 5:{ memcpy(coeff, coeff_quartic_quintic_9,  maskPixels); normalization = coeff_quadr_cubic_9[maskPixels]; break; }
                default:
                break;
            }
        }
    }
    template <typename T>
    void ApplyFilterToSpectr(T* spectr, u::uint32 sizeSpectr, u::uint8* coeff, u::uint8 normalization)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();

        for (u::uint32 i = maskPixels/2; i < sizeSpectr - maskPixels/2; i++)
        {
            T temp;
            for (u::uint8 j = -maskPixels/2; j < maskPixels/2; j++)
            {
                temp+= spectr[i + j]*coeff[j+maskPixels/2];
            }
        }
        spectr[i] = temp/normalization;
    }

    bool ToCube()
    {
        return false;
    }

    bool ToSpectrWindow()
    {
        QVector<double> XUnits = BaseNoiseAlg<T>::m_attributes->GetXUnits();
        QVector<double> spectrMas = BaseNoiseAlg<T>::m_attributes->GetYUnits();
        // определим массив коэффициентов
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        u::int8* coeff = new u::uint8[maskPixels];
        u::int8 normalization;
        ChooseCoef(coeff, normalization);
        // проверить, меняется ли data
        ApplyFilterToSpectr(spectrMas.data(), spectrMas.size(), coeff, normalization);
        delete [] coeff;
        BaseNoiseAlg<T>::m_attributes->ClearUnitsLists();
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFlag(true);
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFormat(0);
        QString descr = QString(" Савитский-Голау фильтр %1x%1").arg(QString::number(BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()));
        BaseNoiseAlg<T>::m_attributes->SetDescriptionItem("Устранение шумов:", descr);
        BaseNoiseAlg<T>::m_attributes->SetXUnit(XUnits);
        BaseNoiseAlg<T>::m_attributes->SetYUnit(spectrMas);
        BaseNoiseAlg<T>::m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(BaseNoiseAlg<T>::m_cube, BaseNoiseAlg<T>::m_attributes);
    }

#endif // SAVITSKIGOLAY1DALG

