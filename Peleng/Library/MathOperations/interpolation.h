#ifndef INTERPOLATION
#define INTERPOLATION

//! функция, проводящая линейную интерполяцию
//! @param Xin - набор длин волн исходного спектра, который нужно преобразовать
//! @param Yin - набор значений отсчетов исходного спектра, который нужно преобразовать
//! @param Xout - целевой набор длин волн, к которым надо привести исходный спектр
//! @param Yout - интерполированные значения отсчетов Xout
//! @return - ф-ия завершилась успешно или нет
bool interpolate(const QVector<double> & Xin, const QVector<double> & Yin,
                 const QVector<double> & Xout, QVector<double> & Yout);


#endif // INTERPOLATION

