#ifndef MEDIAN2DALG
#define MEDIAN2DALG

#include "BaseNoiseAlg.h"

template <typename T>
class Median2DAlg : public BaseNoiseAlg<T>
{
public:
    Median2DAlg(HyperCube* cube, Attributes* attr)
        :BaseNoiseAlg<T>(cube, attr)
    {

    }
    virtual ~Median2DAlg()
    {

    }

    virtual void Execute()
    {
        // реализация
    T* dataChannel = new T[BaseNoiseAlg<T>::m_cube->GetSizeChannel()];

    }
};
#endif // MEDIAN2DALG

