#ifndef READRGBPROFILE_H
#define READRGBPROFILE_H

#include <QObject>
#include <QVector>

class ReadRgbProfile : public QObject
{
    Q_OBJECT
public:
    explicit ReadRgbProfile(QObject *parent = 0);

signals:

public slots:
    void readFromFile(QString FileName);

private:
    QVector<double> wl;
    QVector<double> rChannel;
    QVector<double> gChannel;
    QVector<double> bChannel;

};

#endif // READRGBPROFILE_H
