#ifndef TYPECONVERTOR_H
#define TYPECONVERTOR_H

#include "../Library/Types.h"
#include "../HyperCube.h"

inline void LongLongFromCharArray(const u::int8* data, u::int32 formatType, qint64 &value) {
    union {
        qint64 *eightByte;
        qint32 *fourByte;
        qint16 *twoByte;
        qint8 *oneByte;
//        float *floatType;
    };

    oneByte=(qint8*)data;

    switch (formatType) {
        case type_int8: value = oneByte[0]; break;
        case type_int16: value = twoByte[0]; break;
        case type_int32: value = fourByte[0]; break;
        case type_int64: value = eightByte[0]; break;
//        case type_float: value = floatType[0]; break;
        default: value = 0; break;
    }
}
inline void ULongLongFromCharArray(const u::int8* data, int formatType, quint64 &value) {

    union {
        quint64 *eightUByte;
        quint32 *fourUByte;
        quint16 *twoUByte;
        quint8 *oneUByte;
    };

    oneUByte=(quint8*)data;

    switch (formatType) {
    case type_uint8: value = oneUByte[0]; break;
    case type_uint16: value = twoUByte[0]; break;
    case type_uint32: value = fourUByte[0]; break;
    case type_uint64: value = eightUByte[0]; break;

        default: value = 0; break;
    }
}

inline void FloatFromCharArray(const u::int8* data, float &value) {

    value = *(float*)data;
}
inline void DoubleFromCharArray(const u::int8* data, double &value) {

    value = *(double*)data;
}
#endif // TYPECONVERTOR_H

