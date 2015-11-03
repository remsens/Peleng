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
    , m_attr(attr)
{
    m_ui->setupUi(this);
    QObject::connect(m_ui->pushButton_import, SIGNAL(clicked()), this, SLOT(OnPushButtonImportClicked()));
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
        possibleTitles.append("Name:");
        possibleTitles.append("Type:");
        possibleTitles.append("Class:");
        possibleTitles.append("Subclass:");
        possibleTitles.append("Particle Size:");
        possibleTitles.append("Sample No.:");
        possibleTitles.append("Owner:");
        possibleTitles.append("Wavelength range:");
        possibleTitles.append("Origin:");
        possibleTitles.append("Description:");
        possibleTitles.append("Geologic age:");
        possibleTitles.append("Measurements:");
        possibleTitles.append("First Column:");
        possibleTitles.append("Second Column:");
        possibleTitles.append("X Units:");
        possibleTitles.append("Y Units:");
        possibleTitles.append("First X Value:");
        possibleTitles.append("Last X Value:");
        possibleTitles.append("Number of X Values:");
        possibleTitles.append("Additional Information:");

    } else if (m_ui->radioButton_owner->isChecked())
    {
        possibleTitles.append("Название:");
        possibleTitles.append("Тип:");
        possibleTitles.append("Класс:");
        possibleTitles.append("Подкласс:");
        possibleTitles.append("Владелец:");
        possibleTitles.append("Происхождение:");
        possibleTitles.append("Описание:");
        possibleTitles.append("Измерения:");
        possibleTitles.append("Первый столбец:");
        possibleTitles.append("Второй столбец:");
        possibleTitles.append("Дополнительная информация:");

    }
    QString filePath = QFileDialog::getOpenFileName(this);
    ParseFile(possibleTitles, filePath);
}

void AddSpectr::ParseFile(QStringList &possibleTitles, QString &filePath)
{
    QString err;
    QFile fileIn(filePath);
    try
    {
        if (!fileIn.open(QIODevice::ReadOnly))
        {
            throw GenericExc("Невозможно открыть файл!");
        }

    } catch (const GenericExc& exc)
    {
        err = exc.GetWhat();
    } catch (...)
    {
        err = tr("Неизвестная ошибка");
    }
    if (!err.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", err);
    }
}
