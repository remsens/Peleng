#include "NoisePlugin.h"

#include "AlgFactory.h"
#include "../Library/Types.h"

NoisePlugin::NoisePlugin(QObject *parent)
{

}

NoisePlugin::~NoisePlugin()
{

}

QObject* NoisePlugin::GetObjectPointer()
{
    return this;
}

void NoisePlugin::Execute(HyperCube* cube, Attributes* attr)
{   bool res;
    emit StartOperation(false);
    switch (cube->GetFormatType()) {
    case type_int8:
    {
        res = AlgFactory::I().GetNoiseAlgObject<u::int8>(cube, attr).data()->Execute();
        break;
    }
    case type_int16:
    {
        res = AlgFactory::I().GetNoiseAlgObject<u::int16>(cube, attr).data()->Execute();
        break;
    }
    case type_int32:
    {
        res = AlgFactory::I().GetNoiseAlgObject<u::int32>(cube, attr).data()->Execute();
        break;
    }
    case type_int64:
    {
        res = AlgFactory::I().GetNoiseAlgObject<u::int64>(cube, attr).data()->Execute();
        break;
    }
    case type_uint8:
    {
        res = AlgFactory::I().GetNoiseAlgObject<u::uint8>(cube, attr).data()->Execute();
        break;
    }
    case type_uint16:
    {
        res = AlgFactory::I().GetNoiseAlgObject<u::uint16>(cube, attr).data()->Execute();
        break;
    }
    case type_uint32:
    {
        res = AlgFactory::I().GetNoiseAlgObject<u::uint32>(cube, attr).data()->Execute();
        break;
    }
    case type_float:
    {
        res = AlgFactory::I().GetNoiseAlgObject<float>(cube, attr).data()->Execute();
        break;
    }
    case type_double:
    {
        res = AlgFactory::I().GetNoiseAlgObject<double>(cube, attr).data()->Execute();
        break;
    }
    default:
        break;
    }
    emit FinishOperation(res);
}
