#ifndef MEDIAN1DALG
#define MEDIAN1DALG

#include "BaseNoiseAlg.h"
#include <QElapsedTimer>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include "../Library/Types.h"
#include "../Library/Utils/Compare.h"
#include "../Library/Spectr.h"

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
    void ToSpectrWindow()
    {
        QVector<double> XUnits = BaseNoiseAlg<T>::m_attributes->GetCurrentSpectr()->GetCurrentDataX();

        if (BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()%2 == 0)
        {
            // ошибка, нужно нечетное число
            QMessageBox box;
            box.setText("Неверно выбрано окно пикселей!");
            box.exec();
            return;
        }
        // две реализации алгоритма.
        // Сформировать массив. Для импортируемых - тип double, не проверяем у гиперкуба
        // не думаю, что для спектров так критична скорость

        QVector<double> spectrMas = BaseNoiseAlg<T>::m_attributes->GetCurrentSpectr()->GetCurrentDataY();
        QVector<double> resultSpectr; resultSpectr.resize(spectrMas.size());
        for (u::uint32 i = 0; i < spectrMas.size()- BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()+1; i++)
        {
            // формируем массив
            double* mask = new double[BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()];
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount(); j++)
            {
                mask[j] = spectrMas[i+j]; // формируем маску
            }
            qsort(mask, BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount(), sizeof(double), Compare::CompareVariables<double>);
            resultSpectr.data()[i+BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()/2] = mask[BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()/2];
        }
        spectrMas.clear();
        //BaseNoiseAlg<T>::m_attributes->ClearUnitsLists();
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFlag(true);
        QString descr = QString("Устранение шумов: медианный фильтр %1x%1").arg(QString::number(BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()));
        Measurements measurement;
        BaseNoiseAlg<T>::m_attributes->GetCurrentSpectr()->GetMeasurements(measurement);
        Spectr* spectr = new Spectr(BaseNoiseAlg<T>::m_cube, XUnits, resultSpectr, descr, measurement);
        BaseNoiseAlg<T>::m_attributes->SetCurrentSpectr(spectr);
        BaseNoiseAlg<T>::m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(BaseNoiseAlg<T>::m_cube, BaseNoiseAlg<T>::m_attributes);
    }

    bool ToCube()
    {
        QIcon icon(":/NoiseRemover/icons/NoiseRemover.png");
        u::uint32 size = BaseNoiseAlg<T>::m_cube->GetSizeSpectrum();

        QProgressDialog* progressBar  = new QProgressDialog();
        progressBar->setLabelText("Устранение шумов у гиперспектральных данных");
        progressBar->setWindowIcon(icon);
        QString descr = QString("Медианный фильтр %1x%1").arg(QString::number(BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()));
        progressBar->setWindowTitle(descr);
        progressBar->setRange(0, 100);
        progressBar->setWindowModality(Qt::WindowModal);
        progressBar->setCancelButtonText("Отмена");
        progressBar->show();

        u::uint32 linesCube = BaseNoiseAlg<T>::m_cube->GetLines();
        u::uint32 columnsCube = BaseNoiseAlg<T>::m_cube->GetColumns();
        int percent = 0;
        u::uint32 maxValueBar = linesCube*columnsCube;
        u::uint8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        T* mask = new T[maskPixels];
       // T** dataCube = (T**)BaseNoiseAlg<T>::m_cube->GetDataCube();
        progressBar->setValue(0);
        QApplication::processEvents();
        T* spectr = new T [BaseNoiseAlg<T>::m_cube->GetSizeSpectrum()];
        T* spectrResult = new T [BaseNoiseAlg<T>::m_cube->GetSizeSpectrum()];
        for (u::uint32 lines = 0; lines < linesCube; lines++)
        {
             for (u::uint32 col = 0; col <columnsCube; col++)
             {
                 if (progressBar->wasCanceled())
                 {
                     delete progressBar;
                     //From HDF
                     return false;
                 }
                 BaseNoiseAlg<T>::m_cube->GetSpectrumPoint(lines, col, spectr);
                 memcpy(spectrResult, spectr, BaseNoiseAlg<T>::m_cube->GetSizeSpectrum()*sizeof(T));
                 for (u::uint32 i = 0; i < size - maskPixels+1; i++)
                 {
                     // формируем массив
                     for (u::uint32 j = 0; j < maskPixels; j++)
                     {
                         mask[j] = spectr[i+j]; // формируем маску
                     }
                     qsort(mask, maskPixels, sizeof(T), Compare::CompareVariables<T>);
                     spectrResult[i+maskPixels/2] = mask[maskPixels/2];
                     //BaseNoiseAlg<T>::m_cube->SetDataBuffer(i+maskPixels/2, mask + maskPixels/2, sizeof(T), (lines*columnsCube+col)*sizeof(T));
                 }
                 BaseNoiseAlg<T>::m_cube->SetDataSpectrum(spectrResult, lines, col);
                 double a = lines*columnsCube + col;
                 double b = a/maxValueBar*100;
                 percent = b*100/100;
                 progressBar->setValue(percent);
                 QApplication::processEvents();
             }
         }
         delete [] spectr;
         delete [] spectrResult;
         delete [] mask;
         if (!progressBar->wasCanceled())
         {
             progressBar->setValue(100);
             QApplication::processEvents();
             progressBar->hide();
             delete progressBar;
         }
        return true;
     }
};

#endif // MEDIAN1DALG

