#ifndef INTERPOLATION
#define INTERPOLATION
#include <QVector>

//! функция, проводящая линейную интерполяцию
//! @param X - набор длин волн исходного спектра, который нужно преобразовать (X должен быть отсортирован по возрастанию)
//! @param Y - набор значений отсчетов исходного спектра, который нужно преобразовать
//! @param Xnew - целевой набор длин волн, к которым надо привести исходный спектр (Xnew должен быть отсортирован по возрастанию)
//! @param Ynew - интерполированные значения отсчетов Xnew
//! @return - ф-ия завершилась успешно или нет
bool interpolate(const QVector<double> & X, const QVector<double> & Y,
                 const QVector<double> & Xnew, QVector<double> & Ynew);
bool interpolate(const QVector<double> & X, const QVector<double> & Y,
                 const QList<double> & Xnew, QVector<double> & Ynew);

#endif // INTERPOLATION

