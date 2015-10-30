#ifndef CREATESPECTR_H
#define CREATESPECTR_H

#include <QWidget>
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"
namespace Ui {
class CreateSpectr;
}

class CreateSpectr : public QWidget
{
    Q_OBJECT

public:
    explicit CreateSpectr(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    virtual ~CreateSpectr();

private:
    Ui::CreateSpectr *m_ui;
    HyperCube* m_cube;
    Attributes* m_attributes;
};

#endif // CREATESPECTR_H
