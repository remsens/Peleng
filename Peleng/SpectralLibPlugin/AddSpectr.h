#ifndef ADDSPECTR_H
#define ADDSPECTR_H

#include <QWidget>
#include <QTextStream>
#include <QFile>
#include "../Library/Attributes/Attributes.h"
#include "../Library/HyperCube.h"

class AddSpectr
{

public:
    explicit AddSpectr(HyperCube* cube, Attributes* attr);
    virtual ~AddSpectr();

    void ParseFile();

private:
    HyperCube* m_cube;
    Attributes* m_attr;
    QTextStream* m_inStream;
};

#endif // ADDSPECTR_H
