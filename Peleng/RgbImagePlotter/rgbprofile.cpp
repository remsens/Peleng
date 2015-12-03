#include "rgbprofile.h"
#include <QFile>
#include <QDebug>
#include "cie.h"

RgbProfile::RgbProfile(QObject *parent) : QObject(parent)
{

}

void RgbProfile::readFromFile(QString FileName)
{
    wl.clear();
    rChannel.clear();
    gChannel.clear();
    bChannel.clear();


    QFile file(FileName);
      if (!file.open(QIODevice::ReadOnly)) {
          // TODO
          qDebug() << file.errorString();
          return;
      }

    QList<QByteArray> ByteLine;
    while (!file.atEnd()) {

          QByteArray line = file.readLine();
          ByteLine =  line.replace("\t"," ").trimmed().split(',');
          if (ByteLine.size()==4) {
              wl.append(ByteLine[0].toDouble());
              rChannel.append(ByteLine[1].toDouble());
              gChannel.append(ByteLine[2].toDouble());
              bChannel.append(ByteLine[3].toDouble());
          }
      }

    file.close();

}

void RgbProfile::readStd(RgbProfile::CieProfiles cieProfiles)
{

    wl.clear();
    rChannel.clear();
    gChannel.clear();
    bChannel.clear();
    if (cieProfiles == RgbProfile::CIE1964) {
        for (int i = 0; i < 470; i++) {
            wl.append(cie1964_full[i*4]);
            rChannel.append(cie1964_full[i*4+1]);
            gChannel.append(cie1964_full[i*4+2]);
            bChannel.append(cie1964_full[i*4+3]);
        }
    }
}


