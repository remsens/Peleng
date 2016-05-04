#ifndef FILEPROJECTWINDOW_H
#define FILEPROJECTWINDOW_H

#include <QDialog>
#include <QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "../Library/Attributes/Attributes.h"

namespace Ui {
class FileProjectWindow;
}

class FileProjectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileProjectWindow(Attributes* attr, QDialog *parent = 0);
    ~FileProjectWindow();

public slots:
    void OpenHeaderFile();
    void ChooseTempDir();
    void ChooseSpectralLibDir();
    void CreateFileProject();
signals:
    void SendProjectFilePath(QString);
private:
    Ui::FileProjectWindow *m_ui;

    Attributes* m_attr;
    QString m_headerFile;
    QString m_tempDir;
    QString m_spectralLibDir;
    QString m_projectName;

};

#endif // FILEPROJECTWINDOW_H
