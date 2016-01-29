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
      setAttribute(Qt::WA_DeleteOnClose, false);
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

    // TODO очистка всех плагинов (закрыть все окна)

    if (m_pluginsControl->GetReadingPlugins().size() > 0) {

        cube->DestroyCube();
        FilePlugin = m_pluginsControl->GetReadingPlugins().value("AVIRIS Loader");
        if (FilePlugin != 0)
        {
            QString FileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                     "",
                                                    FilePlugin->getHeaderDescription());
            if (FileName.size() == 0)
            {
                return;
            }
            // Create a progress dialog.
              QProgressDialog dialog(this);
              dialog.setWindowTitle("Тематик-Инфо");
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
              QFuture<void> future = QtConcurrent::run(FilePlugin, &FileReadInterface::readCubeFromFile, FileName, cube);
              // Start the computation.
              futureWatcher.setFuture(future);

              // Display the dialog and start the event loop.
              dialog.exec();

              futureWatcher.waitForFinished();
              dialog.setValue(100);
              dialog.hide();
              timer.stop();
        } else {
            qDebug() << "Плагин AVIRIS Loader не подключен";
            return;
        }
        if (m_canceled)
        {
            cube->DestroyCube();
            m_canceled = false;
        } else {
            if (m_pluginsControl->GetProcessingPlugins().size() > 0)
            {
                m_pelengPlugin = m_pluginsControl->GetProcessingPlugins().value("3DCube UI");
                Attributes::I()->SetAvailablePlugins(m_pluginsControl->GetProcessingPlugins());
                if(cube->GetLines() > 2300)
                {
                    if(cube->GetColumns() > 400)
                        cube->ResizeCube(0,cube->GetCountofChannels()-1,1000,2200,50,400);//чтобы не висла память
                    else
                        cube->ResizeCube(0,cube->GetCountofChannels()-1,1000,2200,0,cube->GetColumns()-1);
                }
                //m_pelengPlugin->Execute(cube, Attributes::I());
                Attributes::I()->SetMaskPixelsCount(3);
                Attributes::I()->ClearList();
                Attributes::I()->SetPoint(0,0, 30);
                Attributes::I()->SetNoiseAlg(Median2D);
                Attributes::I()->SetApplyToAllCube(false);
                Attributes::I()->GetAvailablePlugins().value("Noise Remover")->Execute(cube, Attributes::I());
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


