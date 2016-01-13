#include "CreaterHyperCubes.h"


#include <QObject>
#include <QMessageBox>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include "../Library/HyperCube.h"
#include "../Library/Types.h"

CreaterHyperCubes::CreaterHyperCubes()
{

}

CreaterHyperCubes::~CreaterHyperCubes()
{

}

// Ошибки, получаемые при загрузке фалов можно сделать подробнее
bool CreaterHyperCubes::CreateCube(QString &headerFilePath, HyperCube* cube)
{
    bool res = false;
    res = parseHeaderFile(headerFilePath);
    if (!res)
    {
      QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка чтения файла заголовка %1").arg(headerFilePath), QMessageBox::Ok);
    }
    QString dataFilePath;
    res = getDataFilePath(headerFilePath, dataFilePath);
    if (!res)
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка при получении пути к файлу с данными"), QMessageBox::Ok);
    }
    res = setMetaDataToCube(cube);
    if (!res)
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка записи метаданных"), QMessageBox::Ok);
    }
    res = setMemoryForDataCube(cube);
    if (!res)
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка выделения памяти под данные"), QMessageBox::Ok);
    }
    res = readDataToCube(cube);
    if (!res)
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), QObject::tr("Ошибка чтения данных гиперкуба"), QMessageBox::Ok);
    }
    return res;
}

bool CreaterHyperCubes::parseHeaderFile(QString& headerFilePath)
{

    QFile file(headerFilePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QMap<QString, QString> basedWords;
    basedWords.insert("samples", ""); //0
    basedWords.insert("lines", ""); //1
    basedWords.insert("bands", ""); //2
    basedWords.insert("header", "offset"); //3
    basedWords.insert("data", "type"); //4
    basedWords.insert("interleave", ""); //5
    basedWords.insert("byte", "order"); //6
    basedWords.insert("wavelength", "="); //7
    QString readingLine("");
    u::uint8 wordsPoints = 0;
    while (!file.atEnd())
    {
        readingLine = file.readLine();
        QStringList readWords = readingLine.split(" ");
        for (int k = 0; k < basedWords.size(); k++)
        {
            if (basedWords.contains(readWords.at(0)))
            {
                if (basedWords.value(readWords.at(0)).compare(""))
                {
                    if (QString(readWords.at(0)).compare("samples"))
                    {
                        m_infoData.samples = QString(readWords.at(3)).toLong(); wordsPoints++;
                    } else if (QString(readWords.at(0)).compare("lines"))
                    {
                        m_infoData.lines = QString(readWords.at(3)).toLong(); wordsPoints++;
                    } else if (QString(readWords.at(0)).compare("bands"))
                    {
                        m_infoData.bands = QString(readWords.at(3)).toLong();
                    } else if (QString(readWords.at(0)).compare("interleave")) wordsPoints++;
                    {
                        if (QString(readWords.at(3)).compare("bsq", Qt::CaseInsensitive))
                        {
                            m_interleave = BSQ; wordsPoints++;
                        } else if (QString(readWords.at(3)).compare("bil", Qt::CaseInsensitive))
                        {
                            m_interleave = BIL; wordsPoints++;
                        } else if (QString(readWords.at(3)).compare("bip",Qt::CaseInsensitive))
                        {
                            m_interleave = BIP; wordsPoints++;
                        }
                    }
                } else if (basedWords.value(readWords.at(0)).compare(readWords.at(1)))
                {
                    if (basedWords.key(readWords.at(0)).compare("header") && basedWords.value(readWords.at(0)).compare("offset"))
                    {
                        m_headerOffset = QString(readWords.at(4)).toLong(); wordsPoints++;
                    } else if (basedWords.key(readWords.at(0)).compare("byte") && basedWords.value(readWords.at(0)).compare("order"))
                    {
                        m_byteOrder = QString(readWords.at(4)).toLong(); wordsPoints++;
                    } else if (basedWords.key(readWords.at(0)).compare("data") && basedWords.value(readWords.at(0)).compare("type"))
                    {
                        m_infoData.formatType = TypeFromAvirisType(QString(readWords.at(4)).toLong()); wordsPoints++;
                    } else if (basedWords.key(readWords.at(0)).compare("wavelength") && basedWords.value(readWords.at(0)).compare("="))
                    {
                        readWords.erase(readWords.begin());
                        readWords.erase(readWords.begin());
                        QString w = readWords.join("");
                        readWords.clear();
                        readWords = w.split("{");
                        w = readWords.join("");
                        readWords = w.split(",");
                        for (u::uint32 i = 0; i < (u::uint32)readWords.size(); i++)
                        {
                            m_infoData.listChannels.push_back(QString(readWords.at(i)).toDouble());
                        };
                        u::uint32 k = readWords.size();
                        while (k < m_infoData.bands)
                        {
                            if (!file.atEnd())
                            readingLine = file.readLine();
                            readingLine.split("}");
                            readingLine = readWords.join("");
                            readWords = readingLine.split(",}");
                            for (u::uint32 i = 0; i < (u::uint32)readWords.size(); i++)
                            {
                                m_infoData.listChannels.push_back(QString(readWords.at(i)).toDouble());
                                k++;
                            };
                        }
                    }
                }
                break;
            }
        }
    }
    if (wordsPoints != 7)
    {
        return false;
    } else {
        return true;
    }
}

u::uint32 CreaterHyperCubes::GetNumberOfBytesFromData(u::int32 format) {
    switch (format)
    {
        case type_int8:
        case type_uint8:   return 1;
        case type_int16:
        case type_uint16:  return 2;
        case type_int32:
        case type_uint32:  return 4;
        case type_int64:
        case type_uint64:
        case type_double:  return 8;
        case type_float:   return 4;
        case type_2double: return 16;
        default: return 0;
    }
}

u::uint32 CreaterHyperCubes::TypeFromAvirisType(u::int32 format)
{
    switch (format)
    {
        case 1: return type_int8;
        case 2: return type_int16;
        case 3: return type_int32;
        case 4: return type_float;
        case 5: return type_double;
        case 9: return type_2double;
        case 12: return type_uint16;
        default: return 0;
    }
}


bool CreaterHyperCubes::getDataFilePath(const QString& headerFilePath, QString& dataFilePath)
{
    for (int i = headerFilePath.length()-1; i > 0; i--)
    {
        if (headerFilePath.at(i) != '.')
        {
            dataFilePath.append(headerFilePath.at(i));
            return true;
        }
    }
    return false;
}

bool CreaterHyperCubes::setMetaDataToCube(HyperCube* cube)
{

}

bool CreaterHyperCubes::setMemoryForDataCube(HyperCube* cube)
{

}

bool CreaterHyperCubes::readDataToCube(HyperCube* cube)
{

}
