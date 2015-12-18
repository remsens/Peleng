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
      setAttribute(Qt::WA_DeleteOnClose, true);
      cube = new HyperCube();

      m_pluginsControl = new PluginsControl();
      m_pluginsControl->LoadPlugins();
      ui->mainToolBar->addAction(ui->OpenFileAction);
      ui->mainToolBar->addAction(ui->ExitAction);

      connect(ui->OpenFileAction, SIGNAL(triggered()),SLOT(LoadFile()));
      connect(ui->ExitAction, SIGNAL(triggered()), this, SLOT(close()));
      m_canceled = false;
}


MainWindow::~MainWindow()
{
    delete m_pluginsControl;
    delete ui;
}


void MainWindow::LoadFile()
{

    // TODO
    if (m_pluginsControl->GetReadingPlugins().size() > 0) {

        cube->DestroyCube();
        FilePlugin = m_pluginsControl->GetReadingPlugins().value("AVIRIS Loader");
        FilePlugin->CreateContext();
        if (FilePlugin != 0)
        {
            QString FileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                     "",
                                                    FilePlugin->getFormatDescription());


          // Create a progress dialog.
            QProgressDialog dialog(this);

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
        if (m_canceled)
        {
            cube->DestroyCube();
        } else {
            if (m_pluginsControl->GetProcessingPlugins().size() > 0)
            {
                m_pelengPlugin = m_pluginsControl->GetProcessingPlugins().value("3DCube UI");
                Attributes::I()->SetAvailablePlugins(m_pluginsControl->GetProcessingPlugins());
                cube->ResizeCube(0,223,50,1200,50,400);//чтобы не висла память
                m_pelengPlugin->Execute(cube, Attributes::I());
            }
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
     m_canceled = true;
}


