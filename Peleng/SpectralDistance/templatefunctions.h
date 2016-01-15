#ifndef TEMPLATEFUNCTIONS
#define TEMPLATEFUNCTIONS
#include "../Library/HyperCube.h"
template<typename T>
double evclidFunc(T* dataCube,int i, int j,int line_count,u::uint8 bytesInEl, double spctrPoint)
{
    T value = 0;
    memcpy(&value, dataCube + j*line_count + i, bytesInEl);
    double res = 0;
    res = pow((double)value - spctrPoint, 2);
    return res;
}
template<typename T>
void angleFunc(T* dataCube,int i, int j,int line_count,u::uint8 bytesInEl, double spctrPoint,double& local_val1, double& local_val2, double& local_val3)
{
    //                short int *data_ptr = static_cast<short int*>(m_pHyperCube->GetDataCube()[z]);
    //                local_val1 += (double)data_ptr[j * line_count + i] * yArr.at(z);
    //                local_val2 += pow((double)data_ptr[j * line_count + i],2);
    //                local_val3 += pow(yArr.at(z),2);

    T value = 0;
    memcpy(&value, dataCube + j*line_count + i,bytesInEl);
    local_val1 += (double)value * spctrPoint;
    local_val2 += pow((double)value,2);
    local_val3 += pow(spctrPoint,2);
}



#endif // TEMPLATEFUNCTIONS

