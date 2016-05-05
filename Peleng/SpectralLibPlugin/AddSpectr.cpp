#include "AddSpectr.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <QObject>
#include <QIcon>
#include "../Library/GenericExc.h"

AddSpectr::AddSpectr(HyperCube* cube, Attributes* attr)
    : m_cube(cube)
    , m_attr(attr)
{
}

AddSpectr::~AddSpectr()
{
}

void AddSpectr::ParseFile()
{
    m_attr->ClearUnitsLists();
    QStringList possibleTitles;
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
    //m_attr->SetExternalSpectrFormat(1);
    QString filePath = QFileDialog::getOpenFileName();
    QFile fileIn(filePath);
    m_inStream = new QTextStream(&fileIn);
    m_inStream->setCodec(QTextCodec::codecForName("Windows-1251"));
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
        u::uint32 multiplier = 1;
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
                        if (line.contains("micrometers", Qt::CaseInsensitive) && line.contains("X Units",Qt::CaseInsensitive))
                        {
                            multiplier = 1000;
                        }
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
                    throw GenericExc(QObject::tr("Неизвестный формат данных"));
                } else
                {
                    double x = data.at(0).toDouble()*multiplier;
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
            throw GenericExc(QObject::tr("Неизвестный формат данных"));
        } else if (m_attr->GetYUnits().size() == 0)
        {
            throw GenericExc(QObject::tr("Неизвестный формат данных"));
        } else if (m_attr->GetXUnits().size() != m_attr->GetYUnits().size())
        {
            throw GenericExc(QObject::tr("Неизвестный формат данных"));
        }

        // в некоторых внешних библиотеках длины волн упорядочены по убыванию. Исправляем это
        QVector<double> xForIntOrder;
        QVector<double> yForIntOrder;
        if (m_attr->GetXUnits().first() > m_attr->GetXUnits().last())
        {
            for(int i = 0; i < m_attr->GetXUnits().count(); ++i)
                xForIntOrder.append(m_attr->GetXUnits().at(m_attr->GetXUnits().count() - 1 - i));
            for(int i = 0; i < m_attr->GetYUnits().count(); ++i)
                yForIntOrder.append(m_attr->GetYUnits().at(m_attr->GetYUnits().count() - 1 - i));

            m_attr->SetXUnit(xForIntOrder);
            m_attr->SetYUnit(yForIntOrder);
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
        QMessageBox messageBox;
        QIcon icon(":/pic/icons/information.png");
        messageBox.setWindowIcon(icon);
        messageBox.setWindowTitle("Информация о спектре");
        messageBox.setText(toMessageBox);
        messageBox.exec();
        //QMessageBox::information(NULL, "Информация о спектре", toMessageBox);
        if (m_attr->GetAvailablePlugins().contains("Spectr UI"))
        {
            m_attr->SetExternalSpectrFlag(true);
            m_attr->GetAvailablePlugins().value("Spectr UI")->Execute(m_cube, m_attr);
        }
    } catch (const GenericExc& exc)
    {
        err = exc.GetWhat();
    } catch (...)
    {
        err = QObject::tr("Неизвестная ошибка");
    }
    if (!err.isEmpty())
    {
        QMessageBox::critical(NULL, "Ошибка", err);
    }
}
