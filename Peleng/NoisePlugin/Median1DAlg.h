#ifndef MEDIAN1DALG
#define MEDIAN1DALG

#include "BaseNoiseAlg.h"

#include <QMessageBox>
#include "../Library/Types.h"

int cmp(const void *a, const void *b);

template <typename T>
class Median1DAlg : public BaseNoiseAlg<T>
{
public:
    Median1DAlg(HyperCube* cube, Attributes* attr)
        :BaseNoiseAlg<T>(cube, attr)
    {

    }
    virtual ~Median1DAlg()
    {

    }



public:
    virtual void Execute()
    {
        // сохраним описание спектра и длины волн
        QVector<double> XUnits = BaseNoiseAlg<T>::m_cube->GetListOfChannels().toVector();

        if (BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()%2 == 0)
        {
            // ошибка, нужно нечетное число
            QMessageBox box;
            box.setText("Неверно выбрано окно пикселей!");
            box.exec();
        }
        // две реализации алгоритма.
        // Сформировать массив. Для импортируемых - тип double, не проверяем у гиперкуба
        // не думаю, что для спектров так критична скорость
        QVector<double> spectrMas;
        BaseNoiseAlg<T>::m_cube->GetSpectrumPoint(100, 100, spectrMas);
        for (u::uint32 i = 0; i < spectrMas.size()-BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()+1; i++)
        {
            // формируем массив
            double* mask = new double[BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()];
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount(); j++)
            {
                mask[j] = spectrMas[i+j]; // формируем маску
            }
            qsort(mask, BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount(), sizeof(double), cmp);
            spectrMas.data()[i+BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()/2] = mask[BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()/2];
        }
        BaseNoiseAlg<T>::m_attributes->ClearUnitsLists();
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFlag(true);
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFormat(0);
        BaseNoiseAlg<T>::m_attributes->SetDescriptionItem("Устранение шумов", "Медианный фильтр");
        BaseNoiseAlg<T>::m_attributes->SetXUnit(XUnits);
        BaseNoiseAlg<T>::m_attributes->SetYUnit(spectrMas);
        BaseNoiseAlg<T>::m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(BaseNoiseAlg<T>::m_cube, BaseNoiseAlg<T>::m_attributes);

    }



};

#endif // MEDIAN1DALG

