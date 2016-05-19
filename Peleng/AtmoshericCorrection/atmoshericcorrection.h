#ifndef ATMOSHERICCORRECTION_H
#define ATMOSHERICCORRECTION_H

#include <QObject>
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

class AtmoshericCorrection : public QObject
{
    Q_OBJECT
public:
    explicit AtmoshericCorrection(QObject *parent = 0);


signals:
    void progress();

public slots:
    int doAtmCorr(HyperCube *cube, Attributes *attr);



};

#endif // ATMOSHERICCORRECTION_H
