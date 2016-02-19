#include <QObject>
#include <QVector>

double lerp(double x0, double x1, double y0, double y1, double x) //возвращает y, соответствующий иксу. x0 < x < x1
{
  return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

bool interpolate(const QVector<double> &Xin, const QVector<double> &Yin, const QVector<double> &Xout, QVector<double> &Yout)
{
    int i=0;
    int j=0;



    //поменять местами ин и оут
    while (j < Xin.length() && i < Xout.length())
    {
        if(Xout.at(i) < Xin.at(j) && Xout.at(i+1) > Xin.at(j))
        {
            Yout[j] = lerp(Xout.at(i), Xout.at(i+1), Yout.at(i), Yout.at(i+1), Xin.at(j));
            ++j;
        }
        else
        {
            ++i;
        }
    }

    return true;
}
