#ifndef UTILS_H
#define UTILS_H


class Compare
{
public:
    Compare();
    virtual ~Compare();

    template<typename T>
    static int CompareVariables(const void *a, const void *b)
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
