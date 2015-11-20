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
#include "Utils.h"

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
        for (int i = 0; i < m_listCustomPlot.size(); i++)
        {
            delete m_listCustomPlot.at(i);
        }
        m_listCustomPlot.clear();
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
        T* dataChannel = new T[BaseNoiseAlg<T>::m_cube->GetSizeChannel()];
        BaseNoiseAlg<T>::m_cube->GetDataChannel(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z, dataChannel);
        u::uint32 columns = BaseNoiseAlg<T>::m_cube->GetColumns();
        // фильтр
        u::uint32 pixlWindow = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines() - (pixlWindow + 1)/2; i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns() - (pixlWindow + 1)/2; j++)
            {
                T* arrWindow = new T [pixlWindow*pixlWindow];

                for (u::uint32 k = 0; k < pixlWindow; k++)
                {
                   memcpy(arrWindow + k*pixlWindow, dataChannel +((i+k)*(columns) + j), sizeof(T)*pixlWindow);
                }
                qsort(arrWindow, pixlWindow*pixlWindow, sizeof(T), Utils::Compare<T>);
                dataChannel[(i+1) * (columns) + j +(pixlWindow + 1)/2] = arrWindow [pixlWindow + (pixlWindow + 1)/2];
            }
        }
// отображение без шума
        int minCMap =  32767;
        int maxCMap = -32767;
        T* dataTemp = new T[BaseNoiseAlg<T>::m_cube->GetSizeChannel()];
        memcpy(dataTemp, dataChannel, BaseNoiseAlg<T>::m_cube->GetSizeChannel());
        qsort(dataTemp, BaseNoiseAlg<T>::m_cube->GetSizeChannel(), sizeof(T), Utils::Compare<T>);
        QCustomPlot* customPlot = new QCustomPlot();
       // customPlot->set
        QCPColorMap* colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
        colorMap->data()->setSize(BaseNoiseAlg<T>::m_cube->GetLines(), BaseNoiseAlg<T>::m_cube->GetColumns());
        colorMap->data()->setRange(QCPRange(0, BaseNoiseAlg<T>::m_cube->GetLines()-1), QCPRange(0, BaseNoiseAlg<T>::m_cube->GetColumns()-1));
        customPlot->addPlottable(colorMap);
        for (u::uint32 x = 0; x < BaseNoiseAlg<T>::m_cube->GetLines(); x++) {
            for (u::uint32 y=0; y < BaseNoiseAlg<T>::m_cube->GetColumns(); y++) {
                colorMap->data()->setCell(x, y, dataChannel[x * BaseNoiseAlg<T>::m_cube->GetColumns() + y] );
            }
        }
        minCMap = dataTemp[int(BaseNoiseAlg<T>::m_cube->GetSizeChannel()*0.02)];
        maxCMap = dataTemp[int(BaseNoiseAlg<T>::m_cube->GetSizeChannel()*0.98)];
        customPlot->rescaleAxes();
        delete [] dataTemp;
        colorMap->setDataRange(QCPRange(minCMap,maxCMap));
        colorMap->setGradient(QCPColorGradient::gpGrayscale);
        colorMap->setInterpolate(false);
        customPlot->setInteraction(QCP::iRangeZoom,true);
        customPlot->setInteraction(QCP::iRangeDrag,true);
        customPlot->replot();
        customPlot->show();
        m_listCustomPlot.append(customPlot);
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
             for (u::uint32 i = 0; i < lines - (pixlWindow + 1)/2+1; i++)
             {
                 for (u::uint32 j = 0; j < columns - (pixlWindow + 1)/2+1; j++)
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
                     qsort(arrWindow, pixlWindow*pixlWindow, sizeof(T), Utils::Compare<T>);

                     BaseNoiseAlg<T>::m_cube->SetDataBuffer(ch, arrWindow + (pixlWindow + pixlWindow/2), sizeof(T), ((i+1) * (columns) + j +(pixlWindow)/2)*sizeof(T));
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


private:
    QList<QCustomPlot*> m_listCustomPlot;
};
#endif // MEDIAN2DALG

