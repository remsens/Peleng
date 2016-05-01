#ifndef FILEPROJECTFEATURES_H
#define FILEPROJECTFEATURES_H

#include <QString>
#include <QWidget>

class FileProjectFeatures
{
public:
    FileProjectFeatures();
    virtual ~FileProjectFeatures();

    static bool AddStep(QString& filePath, int stepTag, const QString& tagDescription, QWidget* widget = 0);
    static bool DeleteStep(QString& filePath, int stepTag, QString& tagDescription, QWidget* widget = 0);
};

#endif // FILEPROJECTFEATURES_H
