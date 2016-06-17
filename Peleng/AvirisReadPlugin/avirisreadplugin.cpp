#include "avirisreadplugin.h"
#include <QDebug>

AvirisReadPlugin::AvirisReadPlugin(QObject *parent) :
    QObject(parent)
{
    m_creater = new CreaterHyperCubes();
}


AvirisReadPlugin::~AvirisReadPlugin()
{

}

void AvirisReadPlugin::readCubeFromFile(QString& fileName, HyperCube* cube)
{
    bool res = m_creater->CreateCube(fileName, cube);
    if (!res)
    {
        m_errDescription = m_creater->GetErrorDescription();
    }
}

QString AvirisReadPlugin::getErrorDescription()
{
    return m_errDescription;
}
QString AvirisReadPlugin::getHeaderDescription()
{
    return "Формат заголовков AVIRIS(*.hdr)";
}

void AvirisReadPlugin::cancel()
{
    m_creater->SetCancel();
}
int AvirisReadPlugin::getProgress()
{
    return m_creater->GetProgress();
}
