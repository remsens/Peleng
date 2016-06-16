#include "ReadDataPlugin.h"

ReadDataPlugin::ReadDataPlugin(QObject *parent) :
    QObject(parent)
{
    m_creater = new CreaterHyperCubes();
}


ReadDataPlugin::~ReadDataPlugin()
{

}

void ReadDataPlugin::readCubeFromFile(QString& fileName, HyperCube* cube)
{
    m_creater->CreateCube(fileName, cube);
}

QString ReadDataPlugin::getHeaderDescription()
{
    return "Формат заголовков AVIRIS(*.hdr)";
}

void ReadDataPlugin::cancel()
{
    m_creater->SetCancel();
}
int ReadDataPlugin::getProgress()
{
    return m_creater->GetProgress();
}
