#ifndef UTILS_H
#define UTILS_H


class Utils
{
public:
    Utils();
    virtual ~Utils();

    template<typename T>
    static Compare(const void *a, const void *b)
    {
        const T *pa = (const T*)a;
        const T *pb = (const T*)b;
        if (*pa < *pb)
            return -1;
        else if (*pa > *pb)
            return +1;
        else
            return 0;
    }
};

#endif // UTILS_H
