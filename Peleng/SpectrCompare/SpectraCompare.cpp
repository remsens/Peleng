#include "SpectraCompare.h"
#include "ui_SpectraCompare.h"

SpectraCompare::SpectraCompare(HyperCube* cube, Attributes* attr, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::SpectraCompare)
    , m_cube(cube)
    , m_attribures(attr)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_ui->setupUi(this);

}

SpectraCompare::~SpectraCompare()
{
    delete m_ui;
}
