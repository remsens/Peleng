#include "NoisePlugin.h"

#include "AlgFactory.h"
#include "../Library/Types.h"

NoisePlugin::NoisePlugin(QObject *parent)
{

}

NoisePlugin::~NoisePlugin()
{

}

void NoisePlugin::Execute(HyperCube* cube, Attributes* attr)
{
    switch (cube->GetFormatType()) {
    case type_int8:
    {
        AlgFactory::I().GetNoiseAlgObject<u::int8>(cube, attr).data()->Execute();
        break;
    }
    case type_int16:
    {
        AlgFactory::I().GetNoiseAlgObject<u::int16>(cube, attr).data()->Execute();
        break;
    }
    case type_int32:
    {
        AlgFactory::I().GetNoiseAlgObject<u::int32>(cube, attr).data()->Execute();
        break;
    }
    case type_int64:
    {
        AlgFactory::I().GetNoiseAlgObject<u::int64>(cube, attr).data()->Execute();
        break;
    }
    case type_uint8:
    {
        AlgFactory::I().GetNoiseAlgObject<u::uint8>(cube, attr).data()->Execute();
        break;
    }
    case type_uint16:
    {
        AlgFactory::I().GetNoiseAlgObject<u::uint16>(cube, attr).data()->Execute();
        break;
    }
    case type_uint32:
    {
        AlgFactory::I().GetNoiseAlgObject<u::uint32>(cube, attr).data()->Execute();
        break;
    }
    case type_float:
    {
        AlgFactory::I().GetNoiseAlgObject<float>(cube, attr).data()->Execute();
        break;
    }
    case type_double:
    {
        AlgFactory::I().GetNoiseAlgObject<double>(cube, attr).data()->Execute();
        break;
    }
    default:
        break;
    }
}
