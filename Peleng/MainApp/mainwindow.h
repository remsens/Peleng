#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QListWidgetItem>

#include "../Library/Interfaces/FileReadInterface.h"
#include "HyperCube.h"
#include "PluginsControl.h"
#include "../Library/ReadPluginLoader.h"
#include "../Library/ProcessingPluginLoader.h"
#include <QtXml>
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();


public slots:
    void LoadAvirisData();
    void OpenProject();
    void LoadData();
    void CreateFileProject();
    void takeProjectFile(QString path);

signals:
    void progressValueChanged(int);

private slots:
    void updateProgress();
    void cancelOperation();

private:
    void ShowFileProject();

private:
    Ui::MainWindow *ui;
    FileReadInterface* FilePlugin;
    ProcessingPluginsInterface* m_pelengPlugin;
    HyperCube* cube;
    QMenu *m_pContextMenu;
    PluginsControl* m_pluginsControl;
    bool m_canceled;
    QString m_fileProjectName;
    QMap<QString, QString> m_translatingStepsMap;
};

#endif // MAINWINDOW_H
