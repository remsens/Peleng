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
#include "FileProjectWindow.h"
#include <QDebug>
#include <QMessageBox>
#include "../Library/stepsdefinitions.h"

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

      connect(ui->OpenFileAction, SIGNAL(triggered()),SLOT(OpenProject()));
      connect(ui->ExitAction, SIGNAL(triggered()), this, SLOT(close()));
      connect(ui->pushButton_loadCube, SIGNAL(clicked()), this, SLOT(LoadData()));
      connect(ui->pushButton_loadAvirisData, SIGNAL(clicked()), this, SLOT(LoadAvirisData()));
      connect(ui->CreateProjectAction, SIGNAL(triggered()), this, SLOT(CreateFileProject()));
      m_canceled = false;
      // тут будем хранить список шагов для перевода на русский
      m_translatingStepsMap.insert(MEDIAN_FILTER_FOR_SPECTRA_WITH_STEP, "Медианный фильтр по спектрам с шагом ");
      m_translatingStepsMap.insert(MEDIAN_FILTER_FOR_BANDS_WITH_STEP, "Медианный фильтр по каналам с шагом ");
      m_translatingStepsMap.insert(SAVITSKI_GOLAY_FILTER_WITH_STEP, "Сглаживающий фильтр Савитского-Голая с шагом ");
}


MainWindow::~MainWindow()
{
    delete m_pluginsControl;
    delete ui;
}


void MainWindow::OpenProject()
{
    m_fileProjectName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                             "",
                                            "*.xml");
    if (m_fileProjectName.size() == 0)
    {
        return;
    }

    ShowFileProject();
}

void MainWindow::LoadData()
{
    if (m_pluginsControl->GetReadingPlugins().size() > 0)
    {
        cube->DestroyCube();
        FilePlugin = m_pluginsControl->GetReadingPlugins().value("AVIRIS Loader");
        if (FilePlugin != 0)
        {
            if (Attributes::I()->GetHeaderPath().size() == 0)
            {
                QMessageBox::critical(this, "Ошибка", "Файл-заголовок не может быть пустым");
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
            QFuture<void> future = QtConcurrent::run(FilePlugin, &FileReadInterface::readCubeFromFile, Attributes::I()->GetHeaderPath(), cube);
            // Start the computation.
            futureWatcher.setFuture(future);

            // Display the dialog and start the event loop.
            dialog.exec();

            futureWatcher.waitForFinished();
            dialog.setValue(100);
            dialog.hide();
            timer.stop();
        } else {

            QMessageBox::critical(this, "Ошибка", "Плагин чтения данных Aviris не подключен");
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
                m_pelengPlugin->Execute(cube, Attributes::I());
//
                //Attributes::I()->SetMaskPixelsCount(3);
//              Attributes::I()->ClearList();
//              Attributes::I()->SetPoint(0,0, 30);
//                Attributes::I()->SetNoiseAlg(Median2D);
//                Attributes::I()->SetApplyToAllCube(false);
//                Attributes::I()->GetAvailablePlugins().value("Noise Remover")->Execute(cube, Attributes::I());
            }
        }
    }
}

void MainWindow::LoadAvirisData()
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
                QMessageBox::critical(this, "Ошибка", "Файл-заголовок не может быть пустым");
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
//                if(cube->GetLines() > 2300)
//                {
//                    if(cube->GetColumns() > 400)
//                        cube->ResizeCube(0,cube->GetCountofChannels()-1,1000,2200,50,400);//чтобы не висла память
//                    else
//                        cube->ResizeCube(0,cube->GetCountofChannels()-1,1000,2200,0,cube->GetColumns()-1);
//                }
                m_pelengPlugin->Execute(cube, Attributes::I());
//                Attributes::I()->SetMaskPixelsCount(3);
//                Attributes::I()->ClearList();
//                Attributes::I()->SetPoint(0,0, 30);
//                Attributes::I()->SetNoiseAlg(Median2D);
//                Attributes::I()->SetApplyToAllCube(false);
//                Attributes::I()->GetAvailablePlugins().value("Noise Remover")->Execute(cube, Attributes::I());
            }
        }
    }
}

void MainWindow::CreateFileProject()
{
    FileProjectWindow* fileProject = new FileProjectWindow(Attributes::I());
    QObject::connect(fileProject, SIGNAL(SendProjectFilePath(QString)), this, SLOT(takeProjectFile(QString)));
    fileProject->setModal(true);
    fileProject->show();

}

void MainWindow::takeProjectFile(QString path)
{
    m_fileProjectName = path;
    ShowFileProject();
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

void MainWindow::ShowFileProject()
{
    ui->listWidget_infoProject->clear();
    Attributes::I()->SetFilePathProject(m_fileProjectName);
    QFileInfo fileInfo(m_fileProjectName);
    QString projectName = fileInfo.baseName();
    QFile file(m_fileProjectName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Ошибка", "Невозможно открыть файл-проекта");
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::critical(this, "Ошибка", "Ошибка чтения файла-проекта");
        return;
    }
    file.close();
    QList<QString> steps;
    QDomNode n = doc.firstChild();
    bool correct = false;
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            QString name = e.tagName();
            qDebug() << name;
            if(name == "project")
            {
                n = n.firstChildElement();
                correct = true;
            } else if(name == "headerPath")
            {
                Attributes::I()->SetHeaderPath(e.text());
                correct = true;
                //QString name1 = e.text();
                n = n.nextSiblingElement();
            } else if (name == "tempPath")
            {
                Attributes::I()->SetTempPath(e.text());
                correct = true;
                //QString name1 = e.text();
                n = n.nextSiblingElement();
            } else if (name == "spectralLib")
            {
                Attributes::I()->SetSpectralLibPath(e.text());
                correct = true;
                //QString name1 = e.text();
                n = n.nextSiblingElement();
            } else if (name == "Steps")
            {
                n = n.firstChildElement();
                correct = true;
            } else if (name == "Step")
            {
                //QString name1 = e.text();
                steps.append(e.text());
                n = n.nextSiblingElement();
                correct = true;
            } else
            {
                n = n.nextSiblingElement();
                correct = false;
            }
            if (correct == false)
            {
                QMessageBox::critical(this, "Ошибка", "Неверный формат файла-проекта");
                return;
            }
         }
    }

    setWindowTitle(projectName);
    ui->listWidget_infoProject->addItem(QString("Проект: " ) + projectName);
    ui->listWidget_infoProject->addItem(QString("Путь к файлу-заголовку: " + Attributes::I()->GetHeaderPath()));
    ui->listWidget_infoProject->addItem(QString("Путь к хранению временных файлов: " + Attributes::I()->GetTempPath()));
    ui->listWidget_infoProject->addItem(QString("Путь к каталогу спектральных библиотек: " + Attributes::I()->GetSpectralLibPath()));

    // разбираем шаги
    QStringList trInRussianSteps;
    QList<QString> keys = m_translatingStepsMap.keys();
    for (int i = 0; i < steps.size(); i++)
    {
        for (int j = 0; j < keys.size(); j++)
        {
            if (steps.at(i).contains(keys.at(j), Qt::CaseInsensitive))
            {
                QString temp = steps.at(i).mid(keys.at(j).size()-1, steps.at(i).size()-1 - keys.at(j).size()-1);
                int step = trInRussianSteps.size()+1;
                trInRussianSteps.append("Шаг " + QString::number(step) + ". " + m_translatingStepsMap.value(keys.at(j)) + temp);
            }
        }
    }
    if (trInRussianSteps.size() != steps.size())
    {
        ui->listWidget_infoProject->clear();
        QMessageBox::critical(this, tr("Ошибка"), tr("Неизвестные этапы обработки гиперспектральных данных"));
        return;
    }
    if (trInRussianSteps.size() != 0)
    {
        ui->listWidget_infoProject->addItem("Этапы обработки:");
    }
    foreach (QString str, trInRussianSteps)
    {
        ui->listWidget_infoProject->addItem(str);
    }
}

