#ifndef BASENOISEALG_H
#define BASENOISEALG_H

#include "INoiseAlg.h"
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

template <typename T>
class BaseNoiseAlg: public INoiseAlg<T>
{

public:
    BaseNoiseAlg(HyperCube* cube, Attributes* attr)
        : m_cube(cube)
        , m_attributes(attr)
    {

    }
    virtual ~BaseNoiseAlg()
    {

    }

protected:
    HyperCube* m_cube;
    Attributes* m_attributes;
};

#endif // BASENOISEALG_H
