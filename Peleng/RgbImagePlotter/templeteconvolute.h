#ifndef TEMPLETECONVOLUTE
#define TEMPLETECONVOLUTE

#include "../Library/HyperCube.h"
#include "../Library/GenericExc.h"

#include <QDebug>
#include "cie.h"

// Линейная интерполяция данных. Массив данных должен быть отсортирован.
template<typename T>
double LineInterpolation(double new_wl, QList<double> &wl, T* data) {
    unsigned int index_between_element;

    if ((new_wl>wl[wl.size()-1]) || (new_wl<wl[0])) throw GenericExc("Необходимо экстраполяция, а не интерполяция", -1);
    for (index_between_element = wl.size()-1; index_between_element >0; index_between_element--) {
        if ((new_wl <= wl[index_between_element]) && (new_wl >= wl[index_between_element-1])) break;
    }


    return data[index_between_element-1]+(data[index_between_element]-data[index_between_element-1])/(wl[index_between_element]-wl[index_between_element-1])*(new_wl-wl[index_between_element-1]);

}

template<typename T>
void convolute(HyperCube *cube) {
    int SpectrumSize = cube->GetSizeSpectrum();
    T* data = new T[SpectrumSize];
    QList<double> wl = cube->GetListOfChannels();




    for (int i = 0; wl.size(); i++) {
        cie1964_full[i*4+1]*LineInterpolation(400,wl,data);
    }
    cube->GetSpectrumPoint(400,400,data);

    qDebug() << cie1964_full[1002];

    qDebug() << LineInterpolation(400,wl,data);



/*   for (int i = 0; i < cube->GetLines(); i++) {
        for (int j = 0; j < cube->GetColumns(); j++) {
           cube->GetSpectrumPoint(i,j,data);


        }
    }*/

    delete[] data;


}

#endif // TEMPLETECONVOLUTE

