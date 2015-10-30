#include "CreateSpectr.h"
#include "ui_CreateSpectr.h"

CreateSpectr::CreateSpectr(HyperCube* cube, Attributes* attr, QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::CreateSpectr)
    , m_cube(cube)
    , m_attributes(attr)
{
    m_ui->setupUi(this);
}

CreateSpectr::~CreateSpectr()
{
    delete m_ui;
}
