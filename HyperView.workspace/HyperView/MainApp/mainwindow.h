#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QListWidgetItem>

#include "Interfaces/filereadinterface.h"
#include "../../../Library/HyperCube.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void LoadFile();

signals:
    void progressValueChanged(int);

private slots:
    void updateProgress();
    void tabClose(int index);
    void updateTable();
    void cancelOperation();
    void createPlot();

    void on_ChannelListWidget_itemClicked(QListWidgetItem *item);

private:
    bool loadFilePlugin();
   // void test();

    Ui::MainWindow *ui;

    QList <FileReadInterface*> FileFormatPluginList;
    FileReadInterface* FilePlugin;
    HyperCube* cube;
    QMenu *m_pContextMenu;


};

#endif // MAINWINDOW_H
