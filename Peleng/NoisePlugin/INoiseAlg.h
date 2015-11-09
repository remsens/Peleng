#ifndef INOISEALG
#define INOISEALG

template <typename T>
class INoiseAlg
{
public:
    virtual ~INoiseAlg()
    {

    }
    virtual void Execute() = 0;
};

#endif // INOISEALG

