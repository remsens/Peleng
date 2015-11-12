int cmp(const void *a, const void *b)
{
    const double *pa = (const double*)a;
    const double *pb = (const double*)b;
    if (*pa < *pb)
        return -1;
    else if (*pa > *pb)
        return +1;
    else
        return 0;
}
