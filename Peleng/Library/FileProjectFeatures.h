#ifndef FILEPROJECTFEATURES_H
#define FILEPROJECTFEATURES_H

#include <QString>
#include <QWidget>

class FileProjectFeatures
{
public:
    FileProjectFeatures();
    virtual ~FileProjectFeatures();

    static bool AddStep(const QString& filePath,QWidget* widget = 0);
    static bool DeleteStep(const QString& filePath, QWidget* widget = 0);
};

#endif // FILEPROJECTFEATURES_H
