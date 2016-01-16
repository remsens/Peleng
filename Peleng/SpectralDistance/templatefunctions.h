#ifndef TEMPLATEFUNCTIONS
#define TEMPLATEFUNCTIONS
#include "../Library/HyperCube.h"
template<typename T>
double evclidFunc(T* dataCubeChan,int i, int j,int line_count,u::uint8 bytesInEl, double spctrPoint)
{
    T value = 0;
    memcpy(&value, dataCubeChan + j*line_count + i, bytesInEl);
    double res = 0;
    res = pow((double)value - spctrPoint, 2);
    return res;
}

template<typename T>
void angleFunc(T* dataCubeChan,int i, int j, int line_count,u::uint8 bytesInEl, double spctrPoint
               , double& local_val1, double& local_val2, double& local_val3)
{
    T value = 0;
    memcpy(&value, dataCubeChan + j*line_count + i,bytesInEl);
    local_val1 += (double)value * spctrPoint;
    local_val2 += pow((double)value,2);
    local_val3 += pow(spctrPoint,2);
}


template<typename T>
double averageSpectralValue(T** dataCube, int chan_count, int line_count, int row_count, const int _i, const int _j, bool isInverted)
{
    double chanel_sum = 0;
    for (int z=0; z < chan_count; z++)
    {
        T *data_ptr = dataCube[z];
        if (isInverted)
        {
            chanel_sum += (double)data_ptr[_j*row_count + _i];
        }else
        {
            chanel_sum += (double)data_ptr[_j*line_count + _i];
        }
    }
    return chanel_sum / (double)chan_count;
}
template<typename T>
void corellationFunc(T** dataCube, int i, int j, int chan_count, int line_count, int row_count, double& local_val1
                     , double& local_val2, double& local_val3, QVector<double> yArr, double average_kl)
{
    double average_ij = averageSpectralValue<T>(dataCube,chan_count, line_count, row_count, i,j, false);
    for (int z = 0; z < chan_count; z++)
    {
        T *data_ptr = dataCube[z];
        local_val1 += ((double)data_ptr[j*line_count + i] - average_ij) *
                (yArr.at(z) - average_kl);

        local_val2 += pow((double)data_ptr[j*line_count + i] - average_ij, 2);
        local_val3 += pow(yArr.at(z) - average_kl, 2);
    }
}

#endif // TEMPLATEFUNCTIONS

