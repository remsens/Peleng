#ifndef SPECTRACOMPARE_H
#define SPECTRACOMPARE_H

#include <QMainWindow>
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

namespace Ui {
class SpectraCompare;
}

class SpectraCompare : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpectraCompare(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    virtual ~SpectraCompare();

private:
    Ui::SpectraCompare *m_ui;
    HyperCube* m_cube;
    Attributes* m_attribures;
};

#endif // SPECTRACOMPARE_H
