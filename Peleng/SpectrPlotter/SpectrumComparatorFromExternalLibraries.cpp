#include "SpectrumComparatorFromExternalLibraries.h"

#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include "../Library/HyperCube.h"
#include "SpectrCompareAlg.h"
#include <QProgressDialog>
#include <QApplication>

SpectrumComparatorFromExternalLibraries::SpectrumComparatorFromExternalLibraries()
{

}

SpectrumComparatorFromExternalLibraries::~SpectrumComparatorFromExternalLibraries()
{

}

void SpectrumComparatorFromExternalLibraries::GetSimilarSpectra(HyperCube *cube, Spectr* currentSpectr, int startWave, int endWave, QString startDir, u::uint32 countOfSimilarSpectra, CompareMode mode, QMap<double, Spectr *> &similarSpectra)
{
    // проходим по всем файлам. Узнаем общее количество файлов
    // получили пути ко всем файлам
    QProgressDialog* progressBar  = new QProgressDialog();
    progressBar->setLabelText("Производится анализ файлов в спектральной библиотеке...");
    QString descr = QString("Поиск выбранного спектра в спектральной библиотеке....");
    //progressBar->setWindowIcon(icon);
    progressBar->setWindowTitle(descr);
    progressBar->setRange(0, 100);
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setCancelButtonText("Отменить");
    progressBar->show();
    progressBar->setValue(0);
    QApplication::processEvents();
    QStringList fileNames = getDirFiles(startDir);
    double minCompareValue = 0;
    // формируем спектр. Если размер similarSpectra < countOfSimiralSpectra, то добавляем в вектор
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
    progressBar->setLabelText("Производится загрузка и сравнение спектральных кривых...");
    for (int z = 0; z < fileNames.size(); z++)
    {
        if (progressBar->wasCanceled())
        {
            delete progressBar;
            similarSpectra.clear();
            return;
        }
        QFile fileIn(fileNames.at(z));

        QTextStream* m_inStream = new QTextStream(&fileIn);
        m_inStream->setCodec(QTextCodec::codecForName("Windows-1251"));
        if (!fileIn.open(QIODevice::ReadOnly))
        {
            continue;
        }
        QList<QString> item;
        QVector<double> xUnits;
        QVector<double> yUnits;
        QString title, description;
        DescriptionSpectr descriptionItem;
        QList<DescriptionSpectr> spectrDescription;
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
                    //m_attr->SetDescriptionItem(title, description);
                    descriptionItem.title = title;
                    descriptionItem.description = description;
                    spectrDescription.push_back(descriptionItem);
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
                            descriptionItem.title = title;
                            descriptionItem.description = description;
                            spectrDescription.push_back(descriptionItem);
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
                    continue;
                } else
                {
                    double x = data.at(0).toDouble()*multiplier;
                    double y = data.at(1).toDouble();
                    xUnits.push_back(x);
                    yUnits.push_back(y);
                }
            }
        }
        fileIn.close();
        // проверка на наличие данных
        if (xUnits.size() == 0)
        {
            continue;
        } else if (yUnits.size() == 0)
        {
            continue;
        } else if (xUnits.size() != yUnits.size())
        {
            continue;
        }

        // в некоторых внешних библиотеках длины волн не сортированы по убыванию

        for (int i = 0; i < xUnits.size(); i++)
        {
            bool swapped = false;
            for (int j = 0; j < xUnits.size() -1; j++)
            {
                if (xUnits[j] > xUnits[j+1])
                {
                    double x = xUnits[j];
                    double y = yUnits[j];
                    xUnits[j] = xUnits[j+1];
                    yUnits[j] = yUnits[j+1];
                    xUnits[j+1] = x;
                    yUnits[j+1] = y;
                    swapped = true;
                }
            }
        }
        Measurements measurement = Unknown_measurement;
        for (int j = 0; j < spectrDescription.size(); j++)
        {
            if (spectrDescription.at(j).title.contains("Name", Qt::CaseInsensitive))
            {
                title = spectrDescription.at(j).description;
            } else if (spectrDescription.at(j).title.contains("Y Units", Qt::CaseInsensitive))
            {
                // пробуем определить в чем измеряются величины y
                if (spectrDescription.at(j).description.contains("Reflectance", Qt::CaseInsensitive))
                {
                    // определяем в % или в долях единиц
                    if (spectrDescription.at(j).description.contains("percent", Qt::CaseInsensitive) )
                    {
                        measurement = RFL_percent;
                    } else
                    {
                        measurement = RFL_units;
                    }
                } else if (spectrDescription.at(j).description.contains("Коэф. спектральной яркости", Qt::CaseInsensitive))
                    {
                        measurement = RFL_units;
                    }
            }
        }
        Spectr* spectr = new Spectr(cube, xUnits, yUnits, title, measurement, spectrDescription);
        xUnits.clear(); yUnits.clear(); title.clear(); spectrDescription.clear();
        double result;
        // сравниваем 2 спектра
        spectr->SetCurrentDataType(Spectr::NORMED_INTERPOLATE);
        switch(mode)
        {
            case EvclidDist: result = SpectrCompareAlg::EvclidDistance(currentSpectr->GetCurrentDataY(), spectr->GetCurrentDataY(), startWave, endWave); break;
            case SpectrumAngle: result = SpectrCompareAlg::SpectralAngle(currentSpectr->GetCurrentDataY(), spectr->GetCurrentDataY(), startWave, endWave); break;
            case SpectralCorrelation: result = SpectrCompareAlg::SpectralCorrelation(currentSpectr->GetCurrentDataY(), spectr->GetCurrentDataY(), startWave, endWave); break;
            case SpectrumDist: result = SpectrCompareAlg::SpectralDistance(currentSpectr->GetCurrentDataY(), spectr->GetCurrentDataY(), startWave, endWave); break;
            case SID: result = SpectrCompareAlg::SID(currentSpectr->GetCurrentDataY(), spectr->GetCurrentDataY(), startWave, endWave); break;
            case Entropy: result = SpectrCompareAlg::Entropy(currentSpectr->GetCurrentDataY(), spectr->GetCurrentDataY(), startWave, endWave); break;
        }
        QList<double> keys = similarSpectra.keys();
        qSort(keys.begin(), keys.end());
        if (similarSpectra.size() < countOfSimilarSpectra)
        {
            similarSpectra.insert(result, spectr);
            if (result >= minCompareValue)
            {
                minCompareValue = result;
            }
        } else if (result >= minCompareValue)
        {
               similarSpectra.remove(keys.at(0));
               minCompareValue = result;
               similarSpectra.insert(result, spectr);

        }
        int persent = (double)z/(double)fileNames.size()*100;
        qDebug() << persent;
        progressBar->setValue(persent);
        QApplication::processEvents();
    }
    progressBar->setValue(100);
    QApplication::processEvents();
    progressBar->hide();
    delete progressBar;
    qDebug() << fileNames.size();

}

QStringList SpectrumComparatorFromExternalLibraries::getDirFiles( const QString& dirName  )
{
    QDir dir( dirName );
    QStringList fileNames;
    QStringList fileList = dir.entryList( QDir::Files );
    for ( QStringList::Iterator fit = fileList.begin(); fit != fileList.end(); ++fit )
        fileNames.append( dir.absolutePath() + "/" + *fit );
    QStringList dirList = dir.entryList( QDir::Dirs );
    for ( int i = 0; i < dirList.size(); ++i ) {
        if (dirList.at(i) == "." || dirList.at(i) == "..") {
            continue;
        }
        QDir curDir(dir.absolutePath().append("/").append(dirList.at(i)));

        QStringList curList = getDirFiles( curDir.absolutePath() );
        for ( QStringList::Iterator it = curList.begin(); it != curList.end(); ++it ) fileNames.append( QFileInfo(*it).absoluteFilePath() );
    }
    return fileNames;
}
