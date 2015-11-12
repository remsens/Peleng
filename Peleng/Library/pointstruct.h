#ifndef POINTSTRUCT
#define POINTSTRUCT

#include "Types.h"

//! Структура одного элемента (точки)
 struct Point
 {
     u::uint32 x; //!< Координата x (по строкам)
     u::uint32 y; //!< Координата y (по столбцам)
     u::uint32 z; //!< Координата z (по каналам)
 };
#endif // POINTSTRUCT

