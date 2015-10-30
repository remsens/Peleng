#ifndef ADDSPECTR_H
#define ADDSPECTR_H

#include <QObject>
#include <QWidget>

#include "../Library/Attributes/Attributes.h"
#include "../Library/Interfaces/ProcessingPluginInterface.h"

class AddSpectr
{

public:
    AddSpectr(HyperCube* cube, Attributes* attr);
    virtual ~AddSpectr();

private:
    HyperCube* m_cube;
    Attributes* m_attributes;
};

#endif // ADDSPECTR_H
