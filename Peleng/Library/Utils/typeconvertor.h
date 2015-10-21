#ifndef TYPECONVERTOR_H
#define TYPECONVERTOR_H

inline void LongFromCharArray(char* data, int typesize, qint64 &value) {
    union {
    long long *eightByte;
    int *fourByte;
    short *twoByte;
    char *oneByte;
    };

    oneByte=data;

    switch (typesize) {
        case 1: value = oneByte[0]; break;
        case 2: value = twoByte[0]; break;
        case 4: value = fourByte[0]; break;
        case 8: value = eightByte[0]; break;
        default: value = 0; break;
    }
}

#endif // TYPECONVERTOR_H

