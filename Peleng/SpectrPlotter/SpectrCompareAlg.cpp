#include "SpectrCompareAlg.h"

#include "math.h"

SpectrCompareAlg::SpectrCompareAlg()
{

}

SpectrCompareAlg::~SpectrCompareAlg()
{

}

double SpectrCompareAlg::EvclidDistance(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end)
{
    double res = 0, sum = 0;
    for (int z = start; z < end; z++)
    {
        sum +=pow(dataSpectr1.at(z)- dataSpectr2.at(z), 2);
    }
    res = sqrt(sum);
    return res;
}

double SpectrCompareAlg::SpectralDistance(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end)
{
    double res = 0;
    for (int z = start; z < end; z++)
    {
        res += abs(dataSpectr1.at(z) - dataSpectr2.at(z));
    }
    return res;
}

double SpectrCompareAlg::SpectralAngle(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end)
{
    double res = 0, local_val1 = 0, local_val2 = 0, local_val3 = 0;
    for (int z = start; z < end; z++)
    {
        local_val1 += dataSpectr1.at(z) * dataSpectr2.at(z);
        local_val2 += pow(dataSpectr1.at(z),2);
        local_val3 += pow(dataSpectr2.at(z),2);
    }
    res = acos(local_val1 / (sqrt(local_val2) * sqrt(local_val3)));
    return res;
}

double SpectrCompareAlg::SpectralCorrelation(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end)
{
    double res = 0, local_val1 = 0, local_val2 = 0, local_val3 = 0;
    double chanel_sum1 = 0, chanel_sum2 = 0;
    for (int z=start; z < end; z++)
    {
            chanel_sum1 += dataSpectr1.at(z);
            chanel_sum2 += dataSpectr2.at(z);
    }

    double averageSpectr1 = chanel_sum1/(double)(end-start);
    double averageSpectr2 = chanel_sum2/(double)(end-start);
    for (int z = start; z < end; z++)
    {
        local_val1 += (dataSpectr1.at(z) - averageSpectr1) *
                (dataSpectr2.at(z) - averageSpectr2);
        local_val2 += pow(dataSpectr1.at(z) - averageSpectr1, 2);
        local_val3 += pow(dataSpectr2.at(z) - averageSpectr2, 2);
    }
    res = sqrt(1.0 - local_val1 / (sqrt(local_val2) * sqrt(local_val3)))/sqrt(2);
    return res;
}

double SpectrCompareAlg::SID(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end)
{
    double res = 0, d1 = 0, d2 = 0;
    for (int z = start; z < end; z++)
    {
        if (dataSpectr1.at(z) != 0 && dataSpectr2.at(z) != 0)
        {
            d1 += dataSpectr1.at(z)*log2(dataSpectr1.at(z)/dataSpectr2.at(z));
            d2 += dataSpectr2.at(z)*log2(dataSpectr2.at(z)/dataSpectr1.at(z));
        }
    }
    res = d1+d2;
    return res;
}

double SpectrCompareAlg::Entropy(const QVector<double>& dataSpectr1, const QVector<double>& dataSpectr2, int start, int end)
{
    double res = 0, d1 = 0, d2 = 0;
    for (int z = start; z < end; z++)
    {
        if (dataSpectr1.at(z) != 0 && dataSpectr2.at(z) != 0)
        {
            d1 += dataSpectr1.at(z)/dataSpectr2.at(z);
            d2 += dataSpectr2.at(z)/dataSpectr1.at(z);
        }

    }
    res = log2(d1*d2);
    return res;
}

