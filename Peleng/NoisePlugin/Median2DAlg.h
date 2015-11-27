#ifndef MEDIAN2DALG
#define MEDIAN2DALG

#include "BaseNoiseAlg.h"
#include <QList>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>
#include "../Library/Types.h"
#include "../Library/QCustomPlot.h"
#include "../Library/Utils/Compare.h"
#include "../Library/CustomPlotForPreview2D/Preview2D.h"

template <typename T>
class Median2DAlg : public BaseNoiseAlg<T>
{
public:
    Median2DAlg(HyperCube* cube, Attributes* attr)
        :BaseNoiseAlg<T>(cube, attr)
    {

    }
    virtual ~Median2DAlg()
    {

    }

    virtual bool Execute()
    {

        if (BaseNoiseAlg<T>::m_attributes->GetApplyToAllCube())
        {
            return ToCube();

        } else
        {
            ToWindow();
            return false;
        }

    }
    void ToWindow()
    {
        double* dataChannel = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        QVector<double> data; data.resize(BaseNoiseAlg<T>::m_cube->GetSizeChannel());
        BaseNoiseAlg<T>::m_cube->GetDataChannel(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z, data);
        memcpy(dataChannel, data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        data.clear();
        data.resize(0);
        u::uint32 columns = BaseNoiseAlg<T>::m_cube->GetColumns();
        // фильтр
        u::uint32 pixlWindow = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines() - pixlWindow; i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns() - pixlWindow; j++)
            {
                double* arrWindow = new double [pixlWindow*pixlWindow];

                for (u::uint32 k = 0; k < pixlWindow; k++)
                {
                   memcpy(arrWindow + k*pixlWindow, dataChannel +((i+k)*(columns) + j), sizeof(double)*pixlWindow);
                }
                qsort(arrWindow, pixlWindow*pixlWindow, sizeof(double), Compare::CompareVariables<double>);
                dataChannel[(i+pixlWindow/2) * (columns) + j +pixlWindow/2] = arrWindow [pixlWindow*pixlWindow/2];
                delete [] arrWindow;
            }
        }
        Preview2D* previewWindow = new Preview2D();
        previewWindow->Plot(dataChannel, BaseNoiseAlg<T>::m_cube->GetLines(), BaseNoiseAlg<T>::m_cube->GetColumns(), BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z);
        delete [] dataChannel;
    }

    bool ToCube()
    {
        // предварительная оценка времени:
        QIcon icon(":/NoiseRemover/icons/NoiseRemover.png");
        u::uint32 channels = BaseNoiseAlg<T>::m_cube->GetCountofChannels();
        QProgressDialog* progressBar = new QProgressDialog();
        progressBar->setMinimum(0);
        progressBar->setMaximum(100);
        progressBar->setCancelButtonText("Отмена");
        progressBar->show();
        progressBar->setWindowIcon(icon);
        progressBar->setLabelText("Устранение шумов у гиперспектральных данных");
        QString descr = QString("Медианный фильтр %1x%1").arg(QString::number(BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()));
        progressBar->setWindowTitle(descr);
        u::uint32 percent = 0;
        u::uint32 pixlWindow = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        T* arrWindow = new T [pixlWindow*pixlWindow];
        T** dataCube = (T**)BaseNoiseAlg<T>::m_cube->GetDataCube();
        u::uint32 lines = BaseNoiseAlg<T>::m_cube->GetLines();
        u::uint32 columns = BaseNoiseAlg<T>::m_cube->GetColumns();
        progressBar->setValue(0);
        QApplication::processEvents();
        for (u::uint32 ch  = 0; ch < channels; ch++)
        {
             for (u::uint32 i = 0; i < lines - pixlWindow; i++)
             {
                 for (u::uint32 j = 0; j < columns - pixlWindow; j++)
                 {
                     for (u::uint32 k = 0; k < pixlWindow; k++)
                     {
                         if (progressBar->wasCanceled())
                         {
                              //progressBar->setValue(100);
                              delete progressBar;
                              //From HDF
                              return false;
                          }
                         memcpy(arrWindow + k*pixlWindow, dataCube[ch] +((i+k)*(columns) + j), sizeof(T)*pixlWindow);
                     }
                     qsort(arrWindow, pixlWindow*pixlWindow, sizeof(T), Compare::CompareVariables<T>);
                     BaseNoiseAlg<T>::m_cube->SetDataBuffer(ch, arrWindow + (pixlWindow*pixlWindow/2), sizeof(T), ((i+pixlWindow/2)*columns + j +(pixlWindow)/2)*sizeof(T));
                  }
              }
             double a = (double)((double)ch/channels);
             percent = a*100*100/100;
             progressBar->setValue(percent);
             QApplication::processEvents();
         }
        if (!progressBar->wasCanceled())
        {
            progressBar->setValue(100);
            QApplication::processEvents();
            progressBar->hide();
            delete progressBar;
        }

        //delete progressBar;
        delete [] arrWindow;
        return true;
    }


};
#endif // MEDIAN2DALG

