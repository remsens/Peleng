#include "AddSpectr.h"
#include "ui_AddSpectr.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "../Library/GenericExc.h"

AddSpectr::AddSpectr(HyperCube* cube, Attributes* attr, QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::AddSpectr)
    , m_cube(cube)
    , m_attributes(attr)

{
    m_ui->setupUi(this);
    QObject::connect(m_ui->pushButton_import, SIGNAL(clicked(bool)), this, SLOT(OnPushButtonImportClicked()));
}

AddSpectr::~AddSpectr()
{
    delete m_ui;
}

void AddSpectr::OnPushButtonImportClicked()
{
    QStringList possibleTitles;
    if (m_ui->radioButton_aster->isChecked())
    {
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
    } else if (m_ui->radioButton_owner->isChecked())
    {
        possibleTitles.append(QObject::tr("Название:"));
        possibleTitles.append(QObject::tr("Тип:"));
        possibleTitles.append(QObject::tr("Класс:"));
        possibleTitles.append(QObject::tr("Подкласс:"));
        possibleTitles.append(QObject::tr("Владелец:"));
        possibleTitles.append(QObject::tr("Происхождение:"));
        possibleTitles.append(QObject::tr("Описание:"));
        possibleTitles.append(QObject::tr("Измерения:"));
        possibleTitles.append(QObject::tr("Первый столбец:"));
        possibleTitles.append(QObject::tr("Второй столбец:"));
        possibleTitles.append(QObject::tr("Дополнительная информация:"));
    }
    QString filePath = QFileDialog::getOpenFileName();
    ParseFile(possibleTitles, filePath);
}

void AddSpectr::ParseFile(QStringList& possibleTitles, QString& filePath)
{
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
