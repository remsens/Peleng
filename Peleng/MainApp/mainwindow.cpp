#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QFutureWatcher>
#include <QTimer>
#include <QProgressDialog>
#include <QtConcurrent/QtConcurrent>

#include "../Library/ReadPluginLoader.h"
#include "../Library/ProcessingPluginLoader.h"
#include "../Library/Attributes/Attributes.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
      ui->setupUi(this);
      setWindowIcon(QIcon(":/logo/icons/PelengIcon.png"));

      cube = new HyperCube();

      m_pluginsControl = new PluginsControl();
      m_pluginsControl->LoadPlugins();
      ui->mainToolBar->addAction(ui->OpenFileAction);
      ui->mainToolBar->addAction(ui->ExitAction);

      connect(ui->OpenFileAction,SIGNAL(triggered()),SLOT(LoadFile()));
      connect(ui->ExitAction,SIGNAL(triggered()),this,SLOT(exit()));

}


MainWindow::~MainWindow()
{
    delete m_pluginsControl;
    delete ui;
}


void MainWindow::LoadFile()
{

    // TODO
    if (m_pluginsControl->GetReadingPlugins().size()>0) {

        //Если это не первый вызов, но нужно почистить данные
        //m_pluginsControl->GetReadPlugins().first()->DeleteData();
        //TODO
        FilePlugin = m_pluginsControl->GetReadingPlugins().value("AVIRIS Loader");
        if (FilePlugin != 0)
        {
            QString FileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                     "",
                                                    FilePlugin->getFormatDescription());


          // Create a progress dialog.
            QProgressDialog dialog;

            dialog.setLabelText(QString("Загрузка данных из файла"));

            // Create a QFutureWatcher and connect signals and slots.
            QFutureWatcher<void> futureWatcher;
            QTimer timer;
            connect(&timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
            timer.start(1000);
            QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
            QObject::connect(&dialog, SIGNAL(canceled()), SLOT(cancelOperation()));
            QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
            QObject::connect(this, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

            //extern void FileFormatPluginList[0]->getDataFromChannel(channel,(qint8*)data);

            // TODO

            QFuture<void> future = QtConcurrent::run(FilePlugin, &FileReadInterface::ReadCubeFromFile, FileName, cube);


            // Start the computation.
            futureWatcher.setFuture(future);

            // Display the dialog and start the event loop.
            dialog.exec();

            futureWatcher.waitForFinished();
            dialog.setValue(100);
            dialog.hide();


            timer.stop();
        } else
        {
            qDebug() << "Плагин AVIRIS Loader не подключен";
            return;
        }

        // TODO
            FilePlugin->DeleteData();



        if (m_pluginsControl->GetProcessingPlugins().size() > 0)
        {

            m_pelengPlugin = m_pluginsControl->GetProcessingPlugins().value("3DCube UI");
           // m_pelengPlugin = m_pluginsControl->GetProcessingPlugins().value("SpectralLib UI");
            Attributes::I()->SetAvailablePlugins(m_pluginsControl->GetProcessingPlugins());
           // Attributes::I()->SetModeLib(1);
            cube->ResizeCube(51,223,1922,2447,193,791);
           // cube->ResizeCube(0,224,0,,0,700);
            m_pelengPlugin->Execute(cube, Attributes::I());
        }

    }
}


void MainWindow::updateProgress()
{
    emit progressValueChanged(FilePlugin->getProgress());
}


void MainWindow::cancelOperation()
{
     FilePlugin->cancel();
}


