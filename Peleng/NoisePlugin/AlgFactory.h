#ifndef ALGFACTORY_H
#define ALGFACTORY_H

#include <QSharedPointer>

#include "Median1DAlg.h"
#include "Median2DAlg.h"
#include "../Library/Attributes/Attributes.h"

class AlgFactory
{
public:
    static AlgFactory& I();
    virtual ~AlgFactory();

    template<typename T>
    QSharedPointer<INoiseAlg<T> > GetNoiseAlgObject(HyperCube* cube, Attributes* attr)
    {
        switch (attr->GetNoiseAlg())
        {
            case Median1D:
            {
                QSharedPointer<Median1DAlg<T> >  median1D(new Median1DAlg<T>(cube, attr));
                return median1D;
            }
            case Median2D:
            {
                QSharedPointer<Median2DAlg<T> >  median2D(new Median2DAlg<T>(cube, attr));
                return median2D;
            }
        default:
            {
                QSharedPointer<Median2DAlg<T> >  median2D(new Median2DAlg<T>(cube, attr));
                return median2D;
            }
        }
    }

private:
    AlgFactory();
    AlgFactory(const AlgFactory&);
    AlgFactory& operator=(AlgFactory&);

private:
    static AlgFactory m_instance;
};

#endif // ALGFACTORY_H
