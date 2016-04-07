#ifndef FILEPROJECTFEATURES_H
#define FILEPROJECTFEATURES_H

#include <QString>

class FileProjectFeatures
{
public:
    FileProjectFeatures();
    virtual ~FileProjectFeatures();

    static bool AddStep(QString& filePath, QString& stepTag, QString& tagDescription);
    static bool DeleteStep(QString& filePath, QString& stepTag, QString& tagDescription);
};

#endif // FILEPROJECTFEATURES_H
