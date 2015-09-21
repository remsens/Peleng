#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QJsonObject>
#include <QPluginLoader>
#include <QFileDialog>
#include <QDebug>
#include <QFutureWatcher>
#include <QTimer>
#include <QProgressDialog>
#include <QtConcurrent/QtConcurrent>
#include <QTableView>

#include "../Library/QCustomPlot.h"

class TableModel : public QAbstractTableModel {
private:
    int                          m_nRows;
    int                          m_nColumns;
    qint16*                      db;

public:
    TableModel(int nRows, int nColumns, qint16* dat, QObject* pobj = 0)
        : QAbstractTableModel(pobj)
        , m_nRows(nRows)
        , m_nColumns(nColumns)
    {

        db = new qint16[nRows*nColumns];
        for (int i =0; i< nColumns*nRows; i++) {
                db[i] = dat[i];
            }
    }

    ~TableModel() {
        delete[] db;
    }

    QVariant data(const QModelIndex& index, int nRole) const
    {
        if (!index.isValid()) {
            return QVariant();
        }

        //QString str = QString("%1");
        if( nRole == Qt::DisplayRole ) {
            return db[m_nRows*index.column()+index.row()];
        }

        return QVariant();
    }


    int rowCount(const QModelIndex&) const
    {
        return m_nRows;
    }

    int columnCount(const QModelIndex&) const
    {
        return m_nColumns;
    }

    /*Qt::ItemFlags flags(const QModelIndex &index) const {
        return (Qt::ItemIsSelectable | Qt::ItemIsEnabled)  ^ Qt::ItemIsUserCheckable	;
    }*/


};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    cube = 0;

      m_pluginsControl = new PluginsControl();
      m_pluginsControl->LoadPlugins();
      //ui->tabWidget->setTabsClosable(true);
      ui->mainToolBar->addAction(ui->OpenFileAction);
      ui->mainToolBar->addAction(ui->ExitAction);


      /*m_pContextMenu = new QMenu();

      m_pContextMenu->addAction(ui->CreateTableAction);
      m_pContextMenu->addAction(ui->PlotHeatMapAction);*/




      /*connect(ui->CreateTableAction,SIGNAL(triggered()),SLOT(updateTable()));
      connect(ui->PlotHeatMapAction,SIGNAL(triggered()),SLOT(createPlot()));*/

      connect(ui->OpenFileAction,SIGNAL(triggered()),SLOT(LoadFile()));
      //connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),SLOT(tabClose(int)));
      connect(ui->ExitAction,SIGNAL(triggered()),this,SLOT(exit()));



}


MainWindow::~MainWindow()
{
    //delete m_pContextMenu;
    delete m_pluginsControl;
    delete ui;
}


void MainWindow::LoadFile()
{
    // TODO
    if (m_pluginsControl->GetReadPlugins().size()) {

        //TODO
        QString FileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     m_pluginsControl->GetReadPlugins().first()->getFormatDescription());

        //test();
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
            QFuture<void> future = QtConcurrent::run(m_pluginsControl->GetReadPlugins().first(), &FileReadInterface::LoadFile, FileName);

            // Start the computation.
            futureWatcher.setFuture(future);

            // Display the dialog and start the event loop.
            dialog.exec();

            futureWatcher.waitForFinished();
            dialog.setValue(100);
            dialog.hide();


            timer.stop();


        // TODO
        cube= m_pluginsControl->GetReadPlugins().first()->getCube();

    }

}


void MainWindow::updateProgress()
{
   // TODO
    emit progressValueChanged(m_pluginsControl->GetReadPlugins().first()->getProgress());
}



bool MainWindow::loadFilePlugin()
{
//    QDir pluginsDir(qApp->applicationDirPath());
//#if defined(Q_OS_WIN)
//    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//       // pluginsDir.cdUp();
//#elif defined(Q_OS_MAC)
//    if (pluginsDir.dirName() == "MacOS") {
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//    }
//#endif
//    //pluginsDir.cd("plugins");

//    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
//        if (!fileName.endsWith(".dll")) continue;
//        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
//        QJsonObject MetaData =  pluginLoader.metaData()["MetaData"].toObject();
//        if (MetaData["Type"].toString().contains("FileFormat")) {
//        //qDebug() << pluginLoader.instance();
//        try {
//            QObject *plugin = pluginLoader.instance();
//        if (plugin)
//        {
//            FileFormatPluginList.append(qobject_cast<FileReadInterface *>(plugin));
//           // return true;
//        }
//        }
//            catch (...) {
//                qDebug() << pluginLoader.errorString();
//            }

//        }
//    }
//    return false;
}


void MainWindow::on_ChannelListWidget_itemClicked(QListWidgetItem *item)
{
   // m_pContextMenu->exec(QCursor::pos());
}


void MainWindow::updateTable()
{
   /* qint16 *data =  new qint16[cube->GetColumns()*cube->GetLines()];
    QTableView* table = new QTableView(this);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->addTab(table,QIcon(":/icon/tab/icons/Data Sheet-50.png"),QString("Канал %1").arg(ui->ChannelListWidget->currentItem()->text())));

    qDebug() << "data" << data << cube->GetColumns() << " " << cube->GetLines();
    cube->GetDataChannel(ui->ChannelListWidget->currentRow(),data);
    qDebug() << data;

    TableModel *model=new TableModel(cube->GetColumns(),cube->GetLines(),data);

    table->setModel(model);
    table->show();
    delete[] data;*/

}

void MainWindow::cancelOperation()
{
    // TODO
    m_pluginsControl->GetReadPlugins().first()->cancel();
    //FileFormatPluginList[0]->cancel();
}

void MainWindow::createPlot()
{
     /* QCustomPlot *customPlot = new  QCustomPlot(this);


      customPlot->axisRect()->setAutoMargins(QCP::msNone);
      customPlot->axisRect()->setMargins(QMargins(5,5,5,5));
      QCPColorMap  *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
      customPlot->addPlottable(colorMap);
      colorMap->data()->setSize(cube->GetLines(), cube->GetColumns());
      colorMap->data()->setRange(QCPRange(0, cube->GetLines()), QCPRange(0, cube->GetColumns()));
      customPlot->yAxis->setTicks(false);
      customPlot->xAxis->setTicks(false);
      customPlot->xAxis->setTickLabels(false);
      customPlot->yAxis->setTickLabels(false);
        colorMap->setGradient(QCPColorGradient::gpGrayscale);
      colorMap->setDataRange(QCPRange(0, 2000));

      //QCPColorMapData *MapData
      qint16 *dat =  new qint16[cube->GetColumns()*cube->GetLines()];
      cube->GetDataChannel(ui->ChannelListWidget->currentRow(),dat);


      for (int x=0; x<cube->GetLines(); ++x) {
          for (int y=0; y<cube->GetColumns(); ++y) {

              colorMap->data()->setCell(x, y, dat[x*cube->GetColumns()+y]);

          }
      }



/*      u::uint32 max = 0;

      for (u::uint32 i = 0; i < cube->GetColumns()*cube->GetLines(); i++) {
          u::uint32 counter = 0;*/






      /*std::sort(dat,dat+cube->GetColumns()*cube->GetLines());
      ui->tabWidget->setCurrentIndex(ui->tabWidget->addTab(customPlot,"Изобраажение"));

      colorMap->rescaleDataRange(true);
      colorMap->setDataRange(QCPRange(0,dat[cube->GetColumns()*cube->GetLines()-5]));
      customPlot->rescaleAxes();


      customPlot->replot();
      delete[] dat;*/

}

void MainWindow::tabClose(int index)
{
    /*QWidget* tabItem = ui->tabWidget->widget(index);
       // Removes the tab at position index from this stack of widgets.
       // The page widget itself is not deleted.
       ui->tabWidget->removeTab(index);

       delete(tabItem);*/
}


