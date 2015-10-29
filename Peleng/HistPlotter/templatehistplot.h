#include <QVector>
#include <QProgressDialog>
#include <QTimer>
#include "../Library/HyperCube.h"
#include <climits>

#ifndef TEMPLATEHISTPLOT
#define TEMPLATEHISTPLOT

static const qint32 PROGRESS_PERIOD_UPDATE = 3000; //мс


template<typename T>
void CreateHistValue(HyperCube *cube, int Channel,  int HIST_COUNT, QVector<double>& key, QVector<double>& value) {

    QElapsedTimer timer;
    timer.start();

    quint64 ProgressMax = 0;
    if (Channel<0) ProgressMax = cube->GetSizeCube()*2;
    else ProgressMax = cube->GetSizeChannel()*2;

    QProgressDialog progress("Формирование гистограммы", "Отменить", 0, ProgressMax);
    progress.setWindowModality(Qt::WindowModal);


    T *data = new T[cube->GetSizeChannel()];

    T minValue=std::numeric_limits<T>::max();
    T maxValue=std::numeric_limits<T>::min();


    if (Channel<0) {
        for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
            cube->GetDataChannel(i,data);
            for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
                if (maxValue <= data[j]) maxValue = data[j];
                if (minValue >= data[j]) minValue = data[j];
                if (timer.elapsed() > PROGRESS_PERIOD_UPDATE) {
                    progress.setValue(i*cube->GetSizeChannel() + j*cube->GetBytesInElements());
                    if (progress.wasCanceled()) {
                        progress.setValue(ProgressMax);
                        delete[] data;
                        return;
                    }
                    timer.restart();
                    QApplication::processEvents();

                }
            }

        }
    } else {
        cube->GetDataChannel(Channel,data);
        for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
            if (maxValue <= data[j]) maxValue = data[j];
            if (minValue >= data[j]) minValue = data[j];
            if (timer.elapsed() > PROGRESS_PERIOD_UPDATE) {
                 progress.setValue(j*cube->GetBytesInElements());
                 if (progress.wasCanceled()) {
                    progress.setValue(ProgressMax);
                    delete[] data;
                    return;
                 }
                 timer.restart();
                 QApplication::processEvents();

            }
        }
    }

    progress.setValue(ProgressMax/2);
    T step = (maxValue-minValue)/HIST_COUNT + 1;


    if (Channel<0) {
        for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
                cube->GetDataChannel(i,data);
                for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
                    value[(data[j]-minValue)/(step)]++;
                    if (timer.elapsed() > PROGRESS_PERIOD_UPDATE) {
                        progress.setValue(ProgressMax+i*cube->GetSizeChannel() + j*cube->GetBytesInElements());
                        if (progress.wasCanceled()) {
                            progress.setValue(ProgressMax);
                            delete[] data;
                            return;
                        }
                        timer.restart();
                        QApplication::processEvents();

                    }
                }
        }
    } else {
        cube->GetDataChannel(Channel,data);
        for (unsigned int j = 0; j <cube->GetSizeChannel()/cube->GetBytesInElements(); j++ ) {
            value[(data[j]-minValue)/(step)]++;
            if (timer.elapsed() > PROGRESS_PERIOD_UPDATE) {
                progress.setValue(ProgressMax+j*cube->GetBytesInElements());
                if (progress.wasCanceled()) {
                    progress.setValue(ProgressMax);
                    delete[] data;
                    return;
                }
                timer.restart();
                QApplication::processEvents();
            }
        }
    }

    delete[] data;

    for (int k =0; k<HIST_COUNT+1; k++) {
        key[k]=minValue+k*step;
    }

    progress.setValue(ProgressMax);

}


template<typename T>
void ModifyCube(HyperCube *cube, int Channel, const T& LowBoundary, const T& HighBoundary) {
     T **data = (T**)cube->GetDataCube();

     QTime timer;
     timer.start () ;

    if (Channel<0) {
        QProgressDialog progress("Подождите, пожалуйста", "Отменить", 0, cube->GetSizeCube());
        progress.setWindowModality(Qt::WindowModal);
        for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
            for (unsigned int j = 0; j < cube->GetSizeChannel()/cube->GetBytesInElements(); j++){
                if (data[i][j]<LowBoundary) data[i][j] = LowBoundary;
                if (data[i][j]>HighBoundary) data[i][j] =  HighBoundary;
                if (timer.elapsed() > PROGRESS_PERIOD_UPDATE) {
                    progress.setValue(i*cube->GetSizeChannel() + j*cube->GetBytesInElements());
                    if (progress.wasCanceled()) {
                        progress.setValue(cube->GetSizeCube());

                        return;
                    }
                    timer.restart();
                    QApplication::processEvents();
                }
            }
        }

    } else {
        QProgressDialog progress("Подождите, пожалуйста", "Отменить", 0, cube->GetSizeChannel());
        progress.setWindowModality(Qt::WindowModal);
            for (unsigned int j = 0; j < cube->GetSizeChannel()/cube->GetBytesInElements(); j++){
                if (data[Channel][j] < LowBoundary) data[Channel][j] = LowBoundary;
                if (data[Channel][j] > HighBoundary) data[Channel][j] = HighBoundary;
                if (timer.elapsed() > PROGRESS_PERIOD_UPDATE) {
                    progress.setValue(j*cube->GetBytesInElements());
                    if (progress.wasCanceled()) {
                        progress.setValue(cube->GetSizeCube());
                        return;
                    }
                    timer.restart();
                    QApplication::processEvents();
                }
            }

    }



}


#endif // TEMPLATEHISTPLOT

