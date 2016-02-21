#include "AddSpectr.h"
#include "ui_AddSpectr.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include "../Library/GenericExc.h"
#include "../Library/MathOperations/Interpolation.h"
AddSpectr::AddSpectr(HyperCube* cube, Attributes* attr, QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::AddSpectr)
    , m_cube(cube)
    , m_attr(attr)
{
    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    QObject::connect(m_ui->pushButton_import, SIGNAL(clicked()), this, SLOT(OnPushButtonImportClicked()));
}

AddSpectr::~AddSpectr()
{
    delete m_ui;
}

void AddSpectr::OnPushButtonImportClicked()
{
    m_attr->ClearUnitsLists(); // Убрать! Для тестирования
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
        possibleTitles.append("Wavelength Range:");
        possibleTitles.append("Origin:");
        possibleTitles.append("Description:");
        possibleTitles.append("Geologic age:");
        possibleTitles.append("Measurement:");
        possibleTitles.append("First Column:");
        possibleTitles.append("Second Column:");
        possibleTitles.append("X Units:");
        possibleTitles.append("Y Units:");
        possibleTitles.append("First X Value:");
        possibleTitles.append("Last X Value:");
        possibleTitles.append("Number of X Values:");
        possibleTitles.append("Additional Information:");
        m_attr->SetExternalSpectrFormat(1);

    } else if (m_ui->radioButton_owner->isChecked())
    {
        possibleTitles.append("Название:");
        possibleTitles.append("Тип:");
        possibleTitles.append("Класс:");
        possibleTitles.append("Подкласс:");
        possibleTitles.append("Владелец:");
        possibleTitles.append("Происхождение:");
        possibleTitles.append("Описание:");
        possibleTitles.append("Измеряемая величина:");
        possibleTitles.append("Первый столбец:");
        possibleTitles.append("Второй столбец:");
        possibleTitles.append("Дополнительная информация:");
        m_attr->SetExternalSpectrFormat(0);

    }
    QString filePath = QFileDialog::getOpenFileName(this);
    QFile fileIn(filePath);
    m_inStream = new QTextStream(&fileIn);
    // для русских букв
    if (m_ui->radioButton_owner->isChecked())
    {
        m_inStream->setCodec(QTextCodec::codecForName("Windows-1251"));
    }
    ParseFile(possibleTitles, fileIn);
}

void AddSpectr::ParseFile(QStringList &possibleTitles, QFile &fileIn)
{
    QString err;
    try
    {
        if (!fileIn.open(QIODevice::ReadOnly))
        {
            throw GenericExc("Невозможно открыть файл!");
        }
        QList<QString> item;
        QString title, description;
        // Алгоритм предусматривает, что titles может идти не по порядку. Но нужно как-то разграничивать описание от данных
        // Поэтому смотрим по последнему элементу в possibleTitles;
        bool flagEndDescription = false;
        bool startData = false;
        // костыль, чтобы отличать класс от подкласса
        // bool classTitle = false;
        while (!m_inStream->atEnd())
        {
            QString line = m_inStream->readLine();
            if (line.compare("\r\n") == 0 || line.compare("") == 0 || line.compare(" ") == 0 || line.compare("\n") == 0){
                if (flagEndDescription)
                {
                    // Закидываем последний
                    m_attr->SetDescriptionItem(title, description);
                    item.clear();
                    title.clear();
                    description.clear();
                    startData = true;
                    flagEndDescription = false;
                }
                continue;
            }
            if (!startData)
            {
                bool contains = false;
                for (int i = 0; i < possibleTitles.size(); i++)
                {
                    if (line.contains(possibleTitles.at(i), Qt::CaseInsensitive))
                    {
                        if (line.at(0) != possibleTitles.at(i)[0])
                        {
                            i++;

                        }
                        // закинуть предыдущие и начать новые
                        if (i == possibleTitles.size()-1)
                        {
                            flagEndDescription = true;
                        }
                        if (!title.isEmpty())
                        {
                            m_attr->SetDescriptionItem(title, description);
                            item.clear();
                            title.clear();
                            description.clear();
                        }
                        item = line.split(possibleTitles.at(i), QString::KeepEmptyParts, Qt::CaseInsensitive );
                        title = possibleTitles.at(i);
                        if (item.size() >= 2)
                        {
                            description = item.at(1);
                        } else {
                            description = "";
                        }
                        contains = true;
                        break;
                    }
                }
                if (!contains)
                {
                    description.append(" ").append(line);
                }
            } else {
                item = line.split("\t");
                QList<QString> data;
                for (int i = 0; i < item.size(); i++)
                {
                    QList<QString> it = item.at(i).split(" ");
                    data.append(it);
                }
                for (int i = 0; i < data.size(); i++)
                {
                    if (data.at(i) == "\t" || data.at(i) == "\n" || data.at(i) == " " || data.at(i) == "")
                    {
                        data.removeAt(i);
                        i--;
                    }
                }
                if (data.size() != 2)
                {
                    throw GenericExc(tr("Неизвестный формат данных"));
                } else
                {
                    double x = data.at(0).toDouble();
                    double y = data.at(1).toDouble();
                    m_attr->SetXUnit(x);
                    m_attr->SetYUnit(y);
                }
            }
        }
        fileIn.close();
        // проверка на наличие данных
        if (m_attr->GetXUnits().size() == 0)
        {
            throw GenericExc(tr("Неизвестный формат данных"));
        } else if (m_attr->GetYUnits().size() == 0)
        {
            throw GenericExc(tr("Неизвестный формат данных"));
        } else if (m_attr->GetXUnits().size() != m_attr->GetYUnits().size())
        {
            throw GenericExc(tr("Неизвестный формат данных"));
        }
        // сформировать одну строку
        // QMessageBox;
        QString toMessageBox;
        for (int i = 0; i < m_attr->GetSpectrumDescription().size(); i++)
        {
            toMessageBox.append(m_attr->GetSpectrumDescription().at(i).title);
            toMessageBox.append(m_attr->GetSpectrumDescription().at(i).description);
            toMessageBox.append("\n");
        }
        toMessageBox.append("\n");
        toMessageBox.append("X Units size: ");
        toMessageBox.append(QString::number(m_attr->GetXUnits().size()));
        toMessageBox.append("\n");
        toMessageBox.append("Y Units size: ");
        toMessageBox.append(QString::number(m_attr->GetYUnits().size()));
        QMessageBox::information(this, "Информация о загруженном спектре", toMessageBox);
        if (m_attr->GetAvailablePlugins().contains("Spectr UI"))
        {
            QVector<double> xForTest = m_attr->GetXUnits();
            QVector<double> yForTest = m_attr->GetYUnits();
            if (m_cube->GetListOfChannels().count() > 0)//если нет длин волн, то порядковые номера
            {
                QVector<double> Ynew(m_cube->GetListOfChannels().count());
                QVector<double> xForInt = m_attr->GetXUnits();
                QVector<double> yForInt = m_attr->GetYUnits();
                for(double &it : xForInt) // приводим к нм
                    it = it*1000;

                QVector<double> xForIntOrder;
                QVector<double> yForIntOrder;
                for(int i = 0; i < xForInt.count(); ++i)
                    xForIntOrder.append(xForInt.at(xForInt.count() - 1 - i));
                for(int i = 0; i < yForInt.count(); ++i)
                    yForIntOrder.append(yForInt.at(yForInt.count() - 1 - i));
                interpolate(xForIntOrder, yForIntOrder, m_cube->GetListOfChannels(),Ynew);
                QVector<double> xtemp = m_cube->GetListOfChannels().toVector();
                m_attr->SetXUnit(xtemp);
                m_attr->SetYUnit(Ynew);
                m_attr->GetAvailablePlugins().value("Spectr UI")->Execute(m_cube,m_attr );
            }
            m_attr->SetXUnit(xForTest);
            m_attr->SetYUnit(yForTest);
            m_attr->SetExternalSpectrFlag(true);
            m_attr->GetAvailablePlugins().value("Spectr UI")->Execute(m_cube, m_attr);
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
