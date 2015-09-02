#include "avirisreadplugin.h"

#include <QDebug>



AvirisReadPlugin::AvirisReadPlugin(QObject *parent) :
    QObject(parent)
{
    ctx = 0;
    ReadDataLib_CreateContex(ctx);
}

AvirisReadPlugin::~AvirisReadPlugin()
{
    ReadDataLib_DestroyContex(ctx);
}


void AvirisReadPlugin::LoadFile(QString FileName)
{

    ReadDataLib_LoadFile(ctx, FileName.toStdString().c_str());
    hyperCube = ReadDataLib_CreateHyperCube(ctx);    

}

int AvirisReadPlugin::getProgress()
{      
    return ReadDataLib_GetProgress(ctx);
}

QString AvirisReadPlugin::getFormatDescription()
{
    return "Формат заголовков AVIRIS(*.hdr)";
}

HyperCube *AvirisReadPlugin::getCube()
{

    return  hyperCube;

}

QList<double> AvirisReadPlugin::getListOfChannel()
{

    return QList<double>::fromStdList(ReadDataLib_GetListChannels(ctx));
}


void AvirisReadPlugin::cancel()
{
   // ReadDataLib_BreakOperation(ctx);
}
