#include "AlgFactory.h"

AlgFactory AlgFactory::m_instance;

AlgFactory::AlgFactory()
{

}

AlgFactory& AlgFactory::I()
{
    return m_instance;
}

AlgFactory::~AlgFactory()
{

}


