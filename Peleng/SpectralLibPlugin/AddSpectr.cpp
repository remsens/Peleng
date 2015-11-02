#include "AddSpectr.h"

#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "../Library/GenericExc.h"

AddSpectr::AddSpectr(HyperCube* cube, Attributes* attr)
    : m_cube(cube)
    , m_attributes(attr)
{
    ReadLibrarySpectrAster();
    // пока доступен только aster
    // при необходимости других библиотек, необходимо сделать форму, на которой будет выбор библиотеки
    // aster и другие
}

AddSpectr::~AddSpectr()
{

}

void AddSpectr::ReadLibrarySpectrAster()
{
    QStringList possibleTitles;
    possibleTitles.append(QObject::tr("Name:"));
    possibleTitles.append(QObject::tr("Type:"));
    possibleTitles.append(QObject::tr("Class:"));
    possibleTitles.append(QObject::tr("Subclass:"));
    possibleTitles.append(QObject::tr("Particle Size:"));
    possibleTitles.append(QObject::tr("Sample No.:"));
    possibleTitles.append(QObject::tr("Owner:"));
    possibleTitles.append(QObject::tr("Wavelength range:"));
    possibleTitles.append(QObject::tr("Origin:"));
    possibleTitles.append(QObject::tr("Description:"));
    possibleTitles.append(QObject::tr("Geologic age:"));
    possibleTitles.append(QObject::tr("Measurement:"));
    possibleTitles.append(QObject::tr("First Column:"));
    possibleTitles.append(QObject::tr("Second Column:"));
    possibleTitles.append(QObject::tr("X Units:"));
    possibleTitles.append(QObject::tr("Y Units:"));
    possibleTitles.append(QObject::tr("First X Value:"));
    possibleTitles.append(QObject::tr("Last X Value:"));
    possibleTitles.append(QObject::tr("Number of X Values:"));
    possibleTitles.append(QObject::tr("Additional Information:"));
    QString filePath = QFileDialog::getOpenFileName();
    QFile fileIn(filePath);
    if (filePath.isEmpty())
    {
        return;
    }
    QString err;
    try
    {
        if (!fileIn.open(QIODevice::ReadOnly))
        {
            throw GenericExc(QObject::tr("Ошибка открытия файла"));
        }

    } catch(const GenericExc& exc)
    {
        err = exc.GetWhat();
    } catch (...)
    {
        err = QObject::tr("Неизвестная ошибка");
    }
}
