#ifndef INOISEALG
#define INOISEALG

template <typename T>
class INoiseAlg : public QObject
{
public:
    virtual ~INoiseAlg()
    {

    }
    virtual bool Execute() = 0;
};

#endif // INOISEALG

