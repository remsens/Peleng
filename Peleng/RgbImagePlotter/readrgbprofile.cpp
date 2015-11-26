#include "readrgbprofile.h"

#include <QFile>

ReadRgbProfile::ReadRgbProfile(QObject *parent) : QObject(parent)
{

}

void ReadRgbProfile::readFromFile(QString FileName)
{
    QFile file(FileName);
      if (!file.open(QIODevice::ReadOnly)) {
          qDebug() << file.errorString();
          return 1;
      }

    QStringList wordList;
    while (!file.atEnd()) {

          QByteArray line = file.readLine();
          wordList.append(line.split(',').first());
          qDebug() << wordList;
      }

    file.close();

}

