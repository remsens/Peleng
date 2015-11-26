#ifndef RGBPROFILE_H
#define RGBPROFILE_H

#include <QObject>
#include <QVector>



class RgbProfile : public QObject
{
    Q_OBJECT
public:
    enum CieProfiles{CIE1964};
    explicit RgbProfile(QObject *parent = 0);


signals:

public slots:
    void readFromFile(QString FileName);
    void readStd(RgbProfile::CieProfiles cieProfiles=RgbProfile::CIE1964);
    const QVector<double>* getWl() const { return &wl;}
    const QVector<double>* getRChannel() const { return &rChannel;}
    const QVector<double>* getGChannel() const { return &gChannel;}
    const QVector<double>* getBChannel() const { return &bChannel;}


private:

    QVector<double> wl;
    QVector<double> rChannel;
    QVector<double> gChannel;
    QVector<double> bChannel;

};

#endif // RGBPROFILE_H
