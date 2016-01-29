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
            //ToWindow();
            ToWindowLaplas();
            //ToWindowUolles();
            //ToWindowMedianLaplas();
            return false;
        }

    }
    void ToWindowMedianLaplas()//Лапласс + медианный
    {

        qDebug() << "start borders";
        double* dataChannel = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        double* dataChannelNew = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        QVector<double> data; data.resize(BaseNoiseAlg<T>::m_cube->GetSizeChannel());
        BaseNoiseAlg<T>::m_cube->GetDataChannel(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z, data);
        memcpy(dataChannel, data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        memcpy(dataChannelNew,data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        data.clear();
        data.resize(0);
        u::uint32 columns = BaseNoiseAlg<T>::m_cube->GetColumns();
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
                dataChannelNew[(i+pixlWindow/2) * (columns) + j +pixlWindow/2] = arrWindow [pixlWindow*pixlWindow/2];
                delete [] arrWindow;
            }
        }
        //dataChannelNew теперь входной массив для метода Лапласа
        memcpy(dataChannel, dataChannelNew, BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        // выделение границ (Лаплас)
        pixlWindow = 3;// BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
//        int mtr[3][3] = { -1, -2, -1,
//                          -2, 12, -2,
//                          -1, -2, -1};
        double mtr[3][3] = { 0, -1, 0,
                          -1, 4, -1,
                          0, -1, 0};

        double tempSum = 0;
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines() - pixlWindow; i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns() - pixlWindow; j++)
            {
                for (int k = 0; k < pixlWindow; ++k)
                {
                    for(int m = 0; m < pixlWindow; ++m)
                    {
                        tempSum += dataChannel[(i + k) * columns + j + m] * mtr[k][m];
                    }
                }
                dataChannelNew[(i + (pixlWindow-1)/2) * columns + j + (pixlWindow-1)/2] = tempSum;
                tempSum = 0;
            }
        }
        //---складываем с исходным
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines(); i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns(); j++)
            {
                dataChannelNew[i*columns + j] = dataChannelNew[i*columns + j] + dataChannel[i*columns + j];
            }
        }
        //---
        Preview2D* previewWindow = new Preview2D();
        QString title = QString("Предпросмотр изображения канала: %1 канал").arg(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z);
        previewWindow->Plot(dataChannelNew, BaseNoiseAlg<T>::m_cube->GetLines(), BaseNoiseAlg<T>::m_cube->GetColumns(), title);
        delete [] dataChannel;
        delete [] dataChannelNew;
        qDebug() << "finish borders";
    }
    void ToWindowLaplas()//Лапласс
    {

        qDebug() << "start borders";
        double* dataChannel = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        double* dataChannelNew = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        QVector<double> data; data.resize(BaseNoiseAlg<T>::m_cube->GetSizeChannel());
        BaseNoiseAlg<T>::m_cube->GetDataChannel(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z, data);
        memcpy(dataChannel, data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        memcpy(dataChannelNew,data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        data.clear();
        data.resize(0);
        u::uint32 columns = BaseNoiseAlg<T>::m_cube->GetColumns();
        int pixlWindow = 3;// BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
//        int mtr[3][3] = { -1, -2, -1,
//                          -2, 12, -2,
//                          -1, -2, -1};
        double mtr[3][3] = { 0, -1, 0,
                          -1, 4, -1,
                          0, -1, 0};

        double tempSum = 0;
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines() - pixlWindow; i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns() - pixlWindow; j++)
            {
                for (int k = 0; k < pixlWindow; ++k)
                {
                    for(int m = 0; m < pixlWindow; ++m)
                    {
                        tempSum += dataChannel[(i + k) * columns + j + m] * mtr[k][m];
                    }
                }
                dataChannelNew[(i + (pixlWindow-1)/2) * columns + j + (pixlWindow-1)/2] = tempSum;
                tempSum = 0;
            }
        }
        //---складываем с исходным
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines(); i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns(); j++)
            {
                dataChannelNew[i*columns + j] = dataChannelNew[i*columns + j] + dataChannel[i*columns + j];
            }
        }
        //---
        Preview2D* previewWindow = new Preview2D();
        QString title = QString("Предпросмотр изображения канала: %1 канал").arg(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z);
        previewWindow->Plot(dataChannelNew, BaseNoiseAlg<T>::m_cube->GetLines(), BaseNoiseAlg<T>::m_cube->GetColumns(), title);
        delete [] dataChannel;
        delete [] dataChannelNew;
        qDebug() << "finish borders";
    }
    void ToWindowUolles()//Уоллес
    {

        qDebug() << "start Uolles";
        double* dataChannel = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        double* dataChannelNew = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        QVector<double> data; data.resize(BaseNoiseAlg<T>::m_cube->GetSizeChannel());
        BaseNoiseAlg<T>::m_cube->GetDataChannel(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z, data);
        memcpy(dataChannel, data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        memcpy(dataChannelNew,data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        data.clear();
        data.resize(0);
        u::uint32 columns = BaseNoiseAlg<T>::m_cube->GetColumns();
        // выделение границ (Лаплас)
        u::uint32 pixlWindow = 3;// BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();

        double F = 0;
        double A1 = 0;
        double A3 = 0;
        double A5 = 0;
        double A7 = 0;
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines(); i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns(); j++)
            {
                dataChannelNew[i * columns + j ] = 0;
            }
        }
        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines() - pixlWindow; i++)
        {
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns() - pixlWindow; j++)
            {

                F =  dataChannel[(i + 1) * columns + j + 1];
                A1 = dataChannel[i * columns + j + 1];
                A3 = dataChannel[(i + 1) * columns + j + 2];
                A5 = dataChannel[(i + 2) * columns + j + 1];
                A7 = dataChannel[(i + 1) * columns + j ];
                dataChannelNew[(i + 1) * columns + j + 1] = log(F*F*F*F/A1/A3/A5/A7) / 4;

            }
        }
//        //---складываем с исходным
//        for (u::uint32 i = 0; i < BaseNoiseAlg<T>::m_cube->GetLines(); i++)
//        {
//            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_cube->GetColumns(); j++)
//            {
//                dataChannelNew[i*columns + j] = dataChannelNew[i*columns + j] + 500000*dataChannel[i*columns + j];
//            }
//        }
//        //---
        Preview2D* previewWindow = new Preview2D();
        QString title = QString("Предпросмотр изображения канала: %1 канал").arg(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z);
        previewWindow->Plot(dataChannelNew, BaseNoiseAlg<T>::m_cube->GetLines(), BaseNoiseAlg<T>::m_cube->GetColumns(), title);
        delete [] dataChannel;
        delete [] dataChannelNew;
        qDebug() << "finish Uoles";
    }
    void ToWindow()
    {

        qDebug() << "start noise";
        double* dataChannel = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        double* dataChannelNew = new double[BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double)];
        QVector<double> data; data.resize(BaseNoiseAlg<T>::m_cube->GetSizeChannel());
        BaseNoiseAlg<T>::m_cube->GetDataChannel(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z, data);
        memcpy(dataChannel, data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
        memcpy(dataChannelNew,data.data(), BaseNoiseAlg<T>::m_cube->GetSizeChannel()*sizeof(double));
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
                dataChannelNew[(i+pixlWindow/2) * (columns) + j +pixlWindow/2] = arrWindow [pixlWindow*pixlWindow/2];
                delete [] arrWindow;
            }
        }
        Preview2D* previewWindow = new Preview2D();
        QString title = QString("Предпросмотр изображения канала: %1 канал").arg(BaseNoiseAlg<T>::m_attributes->GetPointsList().at(0).z);
        previewWindow->Plot(dataChannelNew, BaseNoiseAlg<T>::m_cube->GetLines(), BaseNoiseAlg<T>::m_cube->GetColumns(), title);
        delete [] dataChannel;
        delete [] dataChannelNew;
        qDebug() << "finish noise";
    }


    bool ToCube()
    {
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

        delete progressBar;
        delete [] arrWindow;
        return true;
    }


};
#endif // MEDIAN2DALG

