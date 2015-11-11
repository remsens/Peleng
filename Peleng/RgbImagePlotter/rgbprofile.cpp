#include "rgbprofile.h"
#include <QFile>
#include <QDebug>

RgbProfile::RgbProfile(QObject *parent) : QObject(parent)
{

}

void RgbProfile::readFromFile(QString FileName)
{
    QFile file(FileName);
      if (!file.open(QIODevice::ReadOnly)) {
          qDebug() << file.errorString();
          return;
      }

    QStringList wordList;
    while (!file.atEnd()) {

          QByteArray line = file.readLine();
          wordList.append(line.split(',').first());
          qDebug() << wordList;
      }

    file.close();

}

