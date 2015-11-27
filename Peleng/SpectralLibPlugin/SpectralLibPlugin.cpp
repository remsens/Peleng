#include "SpectralLibPlugin.h"

#include "AddSpectr.h"
#include "CreateSpectr.h"

SpectralLibPlugin::SpectralLibPlugin(QObject *parent)
    :QObject(parent)
{

}

SpectralLibPlugin::~SpectralLibPlugin()
{

}
QObject* SpectralLibPlugin::GetObjectPointer()
{
    return this;
}

void SpectralLibPlugin::Execute(HyperCube *cube, Attributes *attr)
{
    if (attr->GetModeLib() == 0)
    {
        CreateSpectr* createSpectr = new CreateSpectr(cube, attr);
        createSpectr->show();
    } else
    {
        attr->ClearUnitsLists();
        AddSpectr* addSpectr = new AddSpectr(cube, attr);
        addSpectr->show();
    }
}
