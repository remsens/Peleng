#include <QVector>

#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"
#include <algorithm>
#include <math.h>

#ifndef TEMPLATEHISTPLOT
#define TEMPLATEHISTPLOT



template<typename T>
void SumBoundary(HyperCube *cube, int Channel,  int HIST_COUNT, QVector<double>& key, QVector<double>& value,Attributes *attr=0,  qint32 LeftBoundary=0, qint32 RightBoundary = 0) {

    //if (attr) attr->ClearList(); //мб эти проверки не нужны?
    T *data = new T[cube->GetSizeChannel()];
    //-----начало
    /*QList<Point> points = attr->GetPointsList();
    for (int i=0; i < points.size(); ++i)
    {
        data[ points.at(i).x * cube->GetColumns() + points.at(i).y] = attr;
    }*/
    memcpy(data,attr->GetTempChanel(),cube->GetSizeChannel()*cube->GetSizeOfFormatType());
    attr->ClearList();
    //-----конец
    //cube->GetDataChannel(Channel,data);

    T maxValue = *std::max_element(data,data+cube->GetSizeChannel());
    T minValue = *std::min_element(data,data+cube->GetSizeChannel());

    if (LeftBoundary==0) LeftBoundary = minValue;
    if (RightBoundary==0) RightBoundary = maxValue;

    T step = (maxValue-minValue)/HIST_COUNT + 1;

    for (unsigned int i = 0; i < cube->GetLines(); i++) {
        for (unsigned int j = 0; j < cube->GetColumns(); j++) {
        if (data[i*cube->GetColumns()+j] < LeftBoundary) data[i*cube->GetColumns()+j]=LeftBoundary;
        if (data[i*cube->GetColumns()+j] > RightBoundary) data[i*cube->GetColumns()+j]=RightBoundary;
        value[(data[i*cube->GetColumns()+j]-minValue)/(step)]++;
        if (attr) attr->SetPoint(i,j,data[i*cube->GetColumns()+j]);
        }
    }


    for (int k =0; k<HIST_COUNT+1; k++) {
        key[k]=minValue+k*step;
    }
    delete[] data;
}



template<typename T>
void Gaussian(HyperCube *cube, int Channel,  int HIST_COUNT, QVector<double>& key, QVector<double>& value, T LeftBoundary, T RightBoundary, Attributes *attr =0) {

    T *data = new T[cube->GetSizeChannel()];
    memcpy(data,attr->GetTempChanel(),cube->GetSizeChannel()*cube->GetSizeOfFormatType());
   // cube->GetDataChannel(Channel,data);
    T maxValue = RightBoundary;
    T minValue = LeftBoundary;
    if (attr) attr->ClearList();

    /*T maxValue = *std::max_element(data,data+cube->GetSizeChannel());
    T minValue = *std::min_element(data,data+cube->GetSizeChannel());*/
    //T step = (maxValue-minValue)/HIST_COUNT + 1;
    double sigma = 0;
    double  mu = 0;


    mu = minValue+(maxValue-minValue)/2;
    sigma = (maxValue-minValue)/3;

    double *temp_data = new double[cube->GetSizeChannel()];


    for (unsigned int j = 0; j <cube->GetSizeChannel(); j++ ) {
        temp_data[j] = data[j] * (1.0/(sigma*sqrt(2*M_PI)))*exp(-(double)pow(data[j]-mu,2)/(2*pow(sigma,2)));
    };


    double maxVal = *std::max_element(temp_data,temp_data+cube->GetSizeChannel());
    double minVal = *std::min_element(temp_data,temp_data+cube->GetSizeChannel());

    for (unsigned int i = 0; i < cube->GetLines(); i++) {
        for (unsigned int j = 0; j < cube->GetColumns(); j++) {

        data[i*cube->GetColumns()+j] = (maxValue - minValue ) * (temp_data[i*cube->GetColumns()+j] - minVal) / (maxVal-minVal)+minValue;
        if (attr) attr->SetPoint(i,j,data[i*cube->GetColumns()+j]);
        }
    }


    double step = (maxValue-minValue)/HIST_COUNT+1;

    for (unsigned int j = 0; j <cube->GetSizeChannel(); j++ ) {
        if (((data[j]-minValue)/step) < 0) continue;
        if (((data[j]-minValue)/step) > value.size()-1) continue;
        value[(data[j]-minValue)/step]++;
    };


    for (int k =0; k<HIST_COUNT+1; k++) {
        key[k]=minValue+k*step;
    }
    delete[] data;
    delete[] temp_data;

}




#endif // TEMPLATEHISTPLOT

