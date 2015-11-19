#ifndef MEDIAN2DALG
#define MEDIAN2DALG

#include "BaseNoiseAlg.h"
#include <QList>
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

    virtual void Execute()
    {

        QPixmap pixmap(":/NoiseRemover/icons/NoiseRemover.png");
        if (BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()%2 == 0)
        {
            // ошибка, нужно нечетное число
            QMessageBox* box = new QMessageBox();
            box->setIconPixmap(pixmap);
            box->setInformativeText(QObject::tr("Ошибка"));
            box->setStandardButtons(QMessageBox::Ok);
            box->setButtonText(QMessageBox::Ok, QObject::tr("Ок"));
            box->setText("Неверно выбрано окно пикселей");
            box->exec();
            delete box;
            return;
        }
        if (BaseNoiseAlg<T>::m_attributes->GetApplyToAllCube())
        {
            ToCube();
        } else
        {
            ToWindow();
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

    void ToCube()
    {
        // предварительная оценка времени:
        QIcon icon(":/NoiseRemover/icons/NoiseRemover.png");
        QPixmap pixmap(":/NoiseRemover/icons/NoiseRemover.png");
        QElapsedTimer* timer = new QElapsedTimer();
        timer->start();
        T* dataChannel = new T[BaseNoiseAlg<T>::m_cube->GetSizeChannel()];
        BaseNoiseAlg<T>::m_cube->GetDataChannel(0, dataChannel);
        u::uint32 lines = BaseNoiseAlg<T>::m_cube->GetLines();
        u::uint32 columns = BaseNoiseAlg<T>::m_cube->GetColumns();
        // фильтр
        u::uint32 pixlWindow = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        for (u::uint32 i = 0; i < lines - (pixlWindow + 1)/2; i++)
        {
            for (u::uint32 j = 0; j < columns - (pixlWindow + 1)/2; j++)
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
        delete [] dataChannel;
        u::uint64 timeOperation = (u::uint64)((u::uint64)timer->elapsed()/1000 + 20)/60;
        delete timer;

        QMessageBox* box = new QMessageBox();
        box->setIconPixmap(pixmap);
        box->setInformativeText(QObject::tr("Предупреждение"));
        box->setStandardButtons(QMessageBox::Yes);
        box->setButtonText(QMessageBox::Yes, QObject::tr("Да"));
        box->setStandardButtons(QMessageBox::No);
        box->setButtonText(QMessageBox::No, QObject::tr("Нет"));
        box->setText(QObject::tr("Приблизительное время обработки данных: %1 минут" "\n Продолжить?").arg(timeOperation));
        u::uint8 n = box->exec();
        if (n == QMessageBox::No)
        {
            return;
        } else
        {
            u::uint32 channels = BaseNoiseAlg<T>::m_cube->GetCountofChannels();
            QProgressBar* progressBar = new QProgressBar();
            progressBar->setMinimum(0);
            progressBar->setMaximum(99);
            progressBar->setTextVisible(true);
            progressBar->show();

            progressBar->setWindowIcon(icon);
            progressBar->setWindowTitle("Устранение шумов у гиперспектральных данных");
            u::uint32 percents = 0;

            for (u::uint32 ch  = 0; ch < channels; ch++)
            {
                dataChannel = new T[BaseNoiseAlg<T>::m_cube->GetSizeChannel()];
                BaseNoiseAlg<T>::m_cube->GetDataChannel(ch, dataChannel);
                for (u::uint32 i = 0; i < lines - (pixlWindow + 1)/2; i++)
                {
                    for (u::uint32 j = 0; j < columns - (pixlWindow + 1)/2; j++)
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
                BaseNoiseAlg<T>::m_cube->SetDataChannel(dataChannel, ch);
                percents = ch/channels*100;
                progressBar->setValue(percents);
                delete [] dataChannel;
            }
            delete progressBar;
        }
    }

private:
    QList<QCustomPlot*> m_listCustomPlot;
};
#endif // MEDIAN2DALG

