#include "atmcorrmodule.h"

#include "atmoshericcorrection.h"



AtmCorrModule::AtmCorrModule(QObject *parent)
{

}

AtmCorrModule::~AtmCorrModule()
{

}
QObject* AtmCorrModule::GetObjectPointer()
{
    return this;
}

void AtmCorrModule::Execute(HyperCube *cube, Attributes *attr)
{
    AtmoshericCorrection atm;
    atm.doAtmCorr(cube,attr);


}
