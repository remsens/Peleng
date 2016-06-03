#include "Interpolation.h"
#include <qdebug>

//Линейная интерполяция по двум точкам.
//возвращает y, соответствующий иксу. x0 < x < x1
double lerp(const double &x0, const double &x1, const double &y0, const double &y1, const double &x)
{
  if (x == x0) // нужно из-за ошибок округления
      return y0;
  else if (x == x1)
      return y1;
  else
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

bool interpolate(const QVector<double> &X, const QVector<double> &Y, const QVector<double> &Xnew, QVector<double> &Ynew)
{
    for (int i = 0; i<X.size()-1; ++i)
    {
        if(X.at(i+1) < X.at(i))
        {
            qDebug()<<"X is unsorted! Error!";
            return false;
        }
    }
    for (int i = 0; i<Xnew.size()-1; ++i)
    {
        if(Xnew.at(i+1) < Xnew.at(i))
        {
            qDebug()<<"Xnew is unsorted! Error!";
            return false;
        }
    }
    Ynew.fill(0); //Если
    int i=0;
    int j=0;
    while (i < X.length()-1 && j < Xnew.length())
    {
        if(X.at(i) <= Xnew.at(j) &&  Xnew.at(j) <= X.at(i+1) )              // x(i) <= Xnew(j) <=x(i+1)
        {
            Ynew[j] = lerp(X.at(i), X.at(i+1), Y.at(i), Y.at(i+1), Xnew.at(j));
            ++j;
        }
        else if (Xnew.at(j) > X.at(i+1))
            ++i;
        else if (Xnew.at(j) < X.at(i))
        {
//            if(i>5 && j>5)
//            {
//                qDebug()<<"X.at(i-2) = "<<X.at(i-2);
//                qDebug()<<"X.at(i-1) = "<<X.at(i-1);
//                qDebug()<<"X.at(i) = "<<X.at(i);
//                qDebug()<<"X.at(i+1) = "<<X.at(i+1);
//                qDebug()<<"Xnew.at(j-1) = "<<Xnew.at(j-1);
//                qDebug()<<"Xnew.at(j) = "<<Xnew.at(j);
//                qDebug()<<"Xnew.at(j+1) = "<<Xnew.at(j+1)<<endl;
//            }
            Ynew[j] = 0;
            ++j;
        }
        else
        {
            qDebug()<<"unexpected 'if' case";
            return false;
        }

    }

    return true;
}


bool interpolate(const QVector<double> &X, const QVector<double> &Y, const QList<double> &Xnew, QVector<double> &Ynew)
{
    QVector<double> XnewVec = Xnew.toVector();
    if(interpolate(X, Y, XnewVec , Ynew))
        return true;
    else
        return false;
}
