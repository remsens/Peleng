#ifndef MEDIAN1DALG
#define MEDIAN1DALG

#include "BaseNoiseAlg.h"
#include <QElapsedTimer>
#include <QMessageBox>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QApplication>
#include "../Library/Types.h"
#include "Utils.h"
#include <QDebug>

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

        if (BaseNoiseAlg<T>::m_attributes->GetApplyToAllCube())
        {
            ToCube();
        } else
        {
            ToSpectrWindow();
        }
    }
public slots:
 void updateProgress()
 {

 }

signals:
    void setProgress();
private:
    void ToSpectrWindow()
    {
        QVector<double> XUnits = BaseNoiseAlg<T>::m_attributes->GetXUnits();

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
        QVector<double> spectrMas = BaseNoiseAlg<T>::m_attributes->GetYUnits();
        for (u::uint32 i = 0; i < spectrMas.size()- BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()+1; i++)
        {
            // формируем массив
            double* mask = new double[BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()];
            for (u::uint32 j = 0; j < BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount(); j++)
            {
                mask[j] = spectrMas[i+j]; // формируем маску
            }
            qsort(mask, BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount(), sizeof(double), Utils::Compare<double>);
            spectrMas.data()[i+BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()/2] = mask[BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()/2];
        }
        BaseNoiseAlg<T>::m_attributes->ClearUnitsLists();
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFlag(true);
        BaseNoiseAlg<T>::m_attributes->SetExternalSpectrFormat(0);
        QString descr = QString(" медианный фильтр %1x%1").arg(QString::number(BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount()));
        BaseNoiseAlg<T>::m_attributes->SetDescriptionItem("Устранение шумов:", descr);
        BaseNoiseAlg<T>::m_attributes->SetXUnit(XUnits);
        BaseNoiseAlg<T>::m_attributes->SetYUnit(spectrMas);
        BaseNoiseAlg<T>::m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(BaseNoiseAlg<T>::m_cube, BaseNoiseAlg<T>::m_attributes);
    }

    void ToCube()
    {
        QPixmap pixmap(":/NoiseRemover/icons/NoiseRemover.png");
        QIcon icon(":/NoiseRemover/icons/NoiseRemover.png");
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
        u::uint32 size = BaseNoiseAlg<T>::m_cube->GetSizeSpectrum();


        // предварительная оценка времени:
        QElapsedTimer* timer = new QElapsedTimer();
        timer->start();
        T* dataSpectrum = new T[size];
        BaseNoiseAlg<T>::m_cube->GetSpectrumPoint(0, 0, dataSpectrum);
        u::uint32 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
        for (u::uint32 i = 0; i < size - maskPixels+1; i++)
        {
            // формируем массив
            T* mask = new T[maskPixels];
            for (u::uint32 j = 0; j < maskPixels; j++)
            {
                mask[j] = dataSpectrum[i+j]; // формируем маску
            }
            qsort(mask, maskPixels, sizeof(T), Utils::Compare<T>);
            dataSpectrum[i+maskPixels/2] = mask[maskPixels/2];
        }
        delete [] dataSpectrum;
        double timeOperation = (timer->elapsed()/1000)*BaseNoiseAlg<T>::m_cube->GetLines()*BaseNoiseAlg<T>::m_cube->GetColumns() + 20;
        timeOperation /= 60;
        delete timer;
        QMessageBox* box = new QMessageBox();
        box->setWindowIcon(icon);
        box->setWindowIconText(QObject::tr("Предупреждение"));
        box->setStandardButtons(QMessageBox::Yes);
        box->setButtonText(QMessageBox::Yes, QObject::tr("Да"));
        box->setStandardButtons(QMessageBox::No);
        box->setButtonText(QMessageBox::No, QObject::tr("Нет"));
        box->setText(QObject::tr("Приблизительное время обработки данных: %1 минут." "Продолжить?").arg((u::uint64)timeOperation));
        u::uint8 n = box->exec();
        if (n == QMessageBox::No)
        {
            return;
        } else
        {
//            QFutureWatcher<void> futureWatcher;
//            QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
//            QObject::connect(&dialog, SIGNAL(canceled()), SLOT(cancelOperation()));
//            QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
//            QObject::connect(this, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

//            QTimer timer;
//            connect(&timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
//            timer.start(1000);

            QProgressDialog* progressBar  = new QProgressDialog();

            progressBar->setLabelText("Устранение шумов у гиперспектральных данных");
            progressBar->setWindowIcon(icon);
            progressBar->setWindowTitle("Медианный фильтр");
            progressBar->setRange(0, 100);
            progressBar->setWindowModality(Qt::WindowModal);
            progressBar->show();

            u::uint32 linesCube = BaseNoiseAlg<T>::m_cube->GetLines();
            u::uint32 columnsCube = BaseNoiseAlg<T>::m_cube->GetColumns();
            int percent = 0;
            u::uint32 maxValueBar = linesCube*columnsCube;
            u::uint8 maskPixels = BaseNoiseAlg<T>::m_attributes->GetMaskPixelsCount();
            T* mask = new T[maskPixels];
            T** dataCube = (T**)BaseNoiseAlg<T>::m_cube->GetDataCube();
            progressBar->setValue(0);
            timer->restart();
            for (u::uint32 lines = 0; lines < linesCube; lines++)
            {
                //qDebug() << "percents " << percent << " line" << lines;
                for (u::uint32 col = 0; col <columnsCube; col++)
                {
                    if (timer->elapsed() > 1000) {

                        progressBar->setValue(percent);
                        if (progressBar->wasCanceled()) {
                            progressBar->setValue(100);
                            //From HDF
                            return;
                        }
                        timer->restart();
                        QApplication::processEvents();
                    }
                    for (u::uint32 i = 0; i < size - maskPixels+1; i++)
                    {
                        // формируем массив
                        for (u::uint32 j = 0; j < maskPixels; j++)
                        {
                            mask[j] = dataCube[i+j][lines*columnsCube+col]; // формируем маску
                        }
                        qsort(mask, maskPixels, sizeof(T), Utils::Compare<T>);
                        BaseNoiseAlg<T>::m_cube->SetDataBuffer(i+maskPixels/2, mask + maskPixels/2, sizeof(T));
                    }
                    double a = lines*columnsCube + col;
                    double b = a/maxValueBar*100;
                    percent = b*100/100;
                    //qDebug() << "lines " << lines << " a " << a << " b " << b << " percent " << (int)percent*100/100;
                    //percent = double((lines*columnsCube + col)/maxValueBar)*100;

                }
            }
            delete [] mask;
            progressBar->setValue(100);
            progressBar->hide();
            delete progressBar;
        }
        delete box;
    }

};

#endif // MEDIAN1DALG

