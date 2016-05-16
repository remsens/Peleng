#include "SpectraComparePlugin.h"

SpectraComparePlugin::SpectraComparePlugin(QObject *parent)
{
    m_spectraCompareWindow = 0;
}

SpectraComparePlugin::~SpectraComparePlugin()
{
    if (m_spectraCompareWindow != 0)
    {
        delete m_spectraCompareWindow;
    }
}

void SpectraComparePlugin::Execute(HyperCube* cube, Attributes* attr)
{
    if (m_spectraCompareWindow == 0)
    {
        m_spectraCompareWindow = new SpectraCompare(cube, attr);
    }
    m_spectraCompareWindow->show();

}

QObject* SpectraComparePlugin::GetObjectPointer()
{
    return this;
}

