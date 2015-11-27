#include "Region.h"

Region::Region()
{
    int limit = 256;
    int randR = qrand() % limit;
    int randG = qrand() % limit;
    int randB = qrand() % limit;
    m_color.setRgb(randR,randG,randB);
    m_name = "Введите название";
}

Region::~Region()
{

}

