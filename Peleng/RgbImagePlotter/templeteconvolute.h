#ifndef TEMPLETECONVOLUTE
#define TEMPLETECONVOLUTE

#include "../Library/HyperCube.h"
#include "../Library/GenericExc.h"


#include <QDebug>
#include "rgbprofile.h"
#include <QImage>
#include <QElapsedTimer>
#include <QProgressDialog>

const qint32 PROGRESS_PERIOD_UPDATE = 1000;

// Линейная интерполяция данных. Массив данных должен быть отсортирован.

double LineInterpolation(double &new_wl, const double* wl, const double* data, int &size_of_data) {
    unsigned int index_between_element;



    if ((new_wl>wl[size_of_data-1]) || (new_wl<wl[0]))  return 0; /*throw GenericExc("Необходимо экстраполяция, а не интерполяция", -1);*/
    for (index_between_element = size_of_data-1; index_between_element >0; index_between_element--) {
        if ((new_wl <= wl[index_between_element]) && (new_wl >= wl[index_between_element-1])) break;
    }


    return data[index_between_element-1]+(data[index_between_element]-data[index_between_element-1])/(wl[index_between_element]-wl[index_between_element-1])*(new_wl-wl[index_between_element-1]);

}

template<typename T>
void RgbChannelPlot(HyperCube *cube, qint32 rCh, qint32 gCh, qint32 bCh, QImage &image) {


    double maxR,maxG,maxB;
    maxR=maxG=maxB=0;
    T** data = (T**)cube->GetDataCube();

    for (unsigned int j = 0; j < cube->GetSizeChannel()/*cube->GetBytesInElements()*/; j++){
        if (data[rCh][j]>=maxR) maxR=data[rCh][j];
        if (data[gCh][j]>=maxG) maxG=data[gCh][j];
        if (data[bCh][j]>=maxB) maxB=data[bCh][j];

    }


    for (unsigned int i = 0; i < cube->GetLines(); i++){
        for (unsigned int j = 0; j < cube->GetColumns(); j++){

            image.setPixel(i,j,qRgb((char)(data[rCh][i*cube->GetColumns()+j]/maxR*255) ,(char)(data[gCh][i*cube->GetColumns()+j]/maxG*255),(char)(data[bCh][i*cube->GetColumns()+j]/maxB*255)));

        }
    }

}


template<typename T>
void RgbProfilePlot(HyperCube *cube, RgbProfile &profile,QImage &image) {

    QElapsedTimer timer;
    timer.start();
    qint32 ProgressMax = cube->GetLines() * cube->GetColumns();
    QProgressDialog progress("Формирование изображения", "Отменить", 0, ProgressMax);
    progress.setWindowModality(Qt::WindowModal);


    QList<double> wl = cube->GetListOfChannels();
    const double* ProfileWl = profile.getWl()->data();
    const double* ProfileRChannel = profile.getRChannel()->data();
    const double* ProfileGChannel = profile.getGChannel()->data();
    const double* ProfileBChannel = profile.getBChannel()->data();
    int ProfileSize = profile.getWl()->size();
    T** data = (T**)cube->GetDataCube();

    double RProfile[cube->GetCountofChannels()];
    double GProfile[cube->GetCountofChannels()];
    double BProfile[cube->GetCountofChannels()];



    for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
        RProfile[i]= LineInterpolation(wl[i], ProfileWl, ProfileRChannel,ProfileSize);
        GProfile[i]= LineInterpolation(wl[i], ProfileWl, ProfileGChannel,ProfileSize);
        BProfile[i]= LineInterpolation(wl[i], ProfileWl, ProfileBChannel,ProfileSize);
    }


    double *RSignal =new double[cube->GetSizeChannel()];
    double *GSignal= new double[cube->GetSizeChannel()];
    double *BSignal= new double[cube->GetSizeChannel()];
    double maxR,maxG,maxB;
    maxR=maxG=maxB=0;




    for (unsigned int j = 0; j < cube->GetSizeChannel()/*cube->GetBytesInElements()*/; j++){
        RSignal[j] = GSignal[j] = BSignal[j] = 0;

        for (unsigned int i = 0; i < cube->GetCountofChannels(); i++) {
            if (data[i][j]>0) {
                RSignal[j] += data[i][j] * RProfile[i];
                GSignal[j] += data[i][j] * GProfile[i];
                BSignal[j] += data[i][j] * BProfile[i];
            }
        }
        if (RSignal[j]>=maxR) maxR=RSignal[j];
        if (GSignal[j]>=maxG) maxG=GSignal[j];
        if (BSignal[j]>=maxB) maxB=BSignal[j];

        if (timer.elapsed() > PROGRESS_PERIOD_UPDATE) {
            progress.setValue(j);
            if (progress.wasCanceled()) {
                progress.setValue(ProgressMax);
                delete[] RSignal;
                delete[] GSignal;
                delete[] BSignal;
                return;
            }
            timer.restart();
            QApplication::processEvents();
        }

    }


    for (unsigned int i = 0; i < cube->GetLines(); i++){
        for (unsigned int j = 0; j < cube->GetColumns(); j++){
            image.setPixel(i,j,qRgb((char)(RSignal[i*cube->GetColumns()+j]/maxR*255) ,(char)(GSignal[i*cube->GetColumns()+j]/maxG*255),(char)(BSignal[i*cube->GetColumns()+j]/maxB*255)));
        }
    }

    progress.setValue(ProgressMax);

    delete[] RSignal;
    delete[] GSignal;
    delete[] BSignal;


}

#endif // TEMPLETECONVOLUTE

