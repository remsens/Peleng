#include "Interpolation.h"
#include <qdebug>

double lerp(const double &x0, const double &x1, const double &y0, const double &y1, const double &x) //возвращает y, соответствующий иксу. x0 < x < x1
{
  return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

bool interpolate(const QVector<double> &X, const QVector<double> &Y, const QVector<double> &Xnew, QVector<double> &Ynew)
{
    int i=0;
    int j=0;
    //поменять местами ин и оут
    while (i < X.length() && j < Xnew.length())
    {
        if(X.at(i) <= Xnew.at(j) &&  Xnew.at(j) <= X.at(i+1) )
        {
            Ynew[j] = lerp(X.at(i), X.at(i+1), Y.at(i), Y.at(i+1), Xnew.at(j));
            ++j;
        }
        else if (X.at(i) <= Xnew.at(j) &&  Xnew.at(j) >= X.at(i+1))
            ++i;
        else if (X.at(i) > Xnew.at(j) &&  Xnew.at(j) < X.at(i+1)) // что-то тут неправильно
        {
            Ynew[j] = 0;
            ++j;
        }
        else
            qDebug()<<"unexpected 'if' case";

    }

    return true;
}


bool interpolate(const QVector<double> &X, const QVector<double> &Y, const QList<double> &Xnew, QVector<double> &Ynew)
{
    QVector<double> XnewVec = Xnew.toVector();
    interpolate(X, Y, XnewVec , Ynew);
}
