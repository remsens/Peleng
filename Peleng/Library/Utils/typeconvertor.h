#ifndef TYPECONVERTOR_H
#define TYPECONVERTOR_H

inline void LongLongFromCharArray(const char* data, int formatType, qint64 &value) {
    union {
        qint64 *eightByte;
        qint32 *fourByte;
        qint16 *twoByte;
        qint8 *oneByte;
    };



    oneByte=(qint8*)data;

    switch (formatType) {
        case 1: value = oneByte[0]; break;
        case 2: value = twoByte[0]; break;
        case 3: value = fourByte[0]; break;
        case 4: value = eightByte[0]; break;
        default: value = 0; break;
    }
}


inline void ULongLongFromCharArray(const char* data, int formatType, quint64 &value) {

    union {
        quint64 *eightUByte;
        quint32 *fourUByte;
        quint16 *twoUByte;
        quint8 *oneUByte;
    };


    oneUByte=(quint8*)data;

    switch (formatType) {
    case 11: value = oneUByte[0]; break;
    case 22: value = twoUByte[0]; break;
    case 33: value = fourUByte[0]; break;
    case 44: value = eightUByte[0]; break;

        default: value = 0; break;
    }
}

#endif // TYPECONVERTOR_H

