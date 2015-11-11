#ifndef RGBPROFILE_H
#define RGBPROFILE_H

#include <QObject>
#include <QVector>

class RgbProfile : public QObject
{
    Q_OBJECT
public:
    explicit RgbProfile(QObject *parent = 0);

signals:

public slots:
    void readFromFile(QString FileName);

private:
    QVector<double> wl;
    QVector<double> rChannel;
    QVector<double> gChannel;
    QVector<double> bChannel;

};

#endif // RGBPROFILE_H
