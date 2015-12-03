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
    u::int32 coeff_quadr_cubic_5[6] = {-3, 12, 17, 12, -3, 35};
    // коэффициенты для полинома x^2 и x^3, окно - 7 пикселей. Последнее значение - значение нормировки
    u::int32 coeff_quadr_cubic_7[8] = {-2, 3, 6, 7, 6, 3, -2, 21};
    // коэффициенты для полинома x^2 и x^3, окно - 9 пикселей. Последнее значение - значение нормировки
    u::int32 coeff_quadr_cubic_9[10] = {-21, 14, 39, 54, 59, 54, 39, 14, -21, 231};
    // коэффициенты для полинома x^4 и x^5, окно - 7 пикселей. Последнее значение - значение нормировки
    u::int32 coeff_quartic_quintic_7[8] = {5, -30, 75, 131, 75, -30, 5, 231};
    // коэффициенты для полинома x^4 и x^5, окно - 9 пикселей. Последнее значение - значение нормировки
    u::int32 coeff_quartic_quintic_9[10] = {15, -55, 30, 135, 179, 135, 30, -55, 15, 429};

private:

    void ChooseCoef(u::int32* coeff, u::uint32& normalization)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        switch (maskPixels)
        {
            case 5:
            {
             memcpy(coeff, coeff_quadr_cubic_5,  maskPixels*sizeof(u::int32));
             normalization = coeff_quadr_cubic_5[maskPixels]; break;
            }

            case 7:
            {
                switch(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom())
                {
                    case 2:
                    case 3: { memcpy(coeff, coeff_quadr_cubic_7,  maskPixels*sizeof(u::int32)); normalization = coeff_quadr_cubic_7[maskPixels]; break; }
                    case 4:
                    case 5:{ memcpy(coeff, coeff_quartic_quintic_7,  maskPixels*sizeof(u::int32)); normalization = coeff_quartic_quintic_7[maskPixels]; break; }
                    default: break;
                }
                break;
            }
            case 9:
            {
                switch(BaseNoiseAlg<T>::m_attributes->GetDegreePolinom())
                {
                    case 2:
                    case 3: { memcpy(coeff, coeff_quadr_cubic_9,  maskPixels*sizeof(u::int32)); normalization = coeff_quadr_cubic_9[maskPixels]; break; }
                    case 4:
                    case 5:{ memcpy(coeff, coeff_quartic_quintic_9,  maskPixels*sizeof(u::int32)); normalization = coeff_quartic_quintic_9[maskPixels]; break; }
                    default:  break;
                }
                break;
            }
        }
    }

    template <typename M>
    void ApplyFilterToSpectr(M* spectr, u::uint32 sizeSpectr, u::int32* coeff, u::uint32 normalization)
    {
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();

        for (u::uint32 i = maskPixels/2; i < sizeSpectr - maskPixels/2; i++)
        {
            M temp = 0;
            for (u::uint8 j = 0; j < maskPixels; j++)
            {
                temp+= spectr[i + j-maskPixels/2]*coeff[j];
            }
            spectr[i] = temp/normalization;
        }

    }

    bool ToCube()
    {
        return false;
    }

    void ToSpectrWindow()
    {
        QVector<double> XUnits = BaseNoiseAlg<T>::m_attributes->GetXUnits();
        QVector<double> spectrMas = BaseNoiseAlg<T>::m_attributes->GetYUnits();
        // определим массив коэффициентов
        u::int8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        u::int32* coeff = new u::int32[maskPixels];
        u::uint32 normalization;
        ChooseCoef(coeff, normalization);
        // проверить, меняется ли data
        ApplyFilterToSpectr<double>(spectrMas.data(), spectrMas.size(), coeff, normalization);
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
};
#endif // SAVITSKIGOLAY1DALG

