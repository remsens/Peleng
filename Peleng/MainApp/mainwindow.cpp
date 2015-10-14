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
#include "../Library/PluginAttributes/SpectrPluginAttributes.h"
#include "../Library/PluginAttributes/Cube3DPluginAttributes.h"
#include "../Library/PluginAttributes/LinePluginAttributes.h"
#include "../Library/PluginAttributes/Cube3DPluginAttributes.h"
#include "../Library/ReadPluginLoader.h"
#include "../Library/PelengPluginLoader.h"
#include "../Library/PluginAttributes/ContextMenu/PureContextMenu.h"

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
      setWindowIcon(QIcon(":/logo/icons/PelengIcon.png"));

      m_cube = new HyperCube();

      m_pluginsControl = new PluginsControl();
      m_pluginsControl->LoadNamesPlugins();
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

        //Если это не первый вызов, но нужно почистить данные
        //m_pluginsControl->GetReadPlugins().first()->DeleteData();
        //TODO
        ReadPluginLoader readPlugin;
        FilePlugin = readPlugin.LoadPlugin(m_pluginsControl->GetReadPlugins().firstKey());
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


            // TODO
            QFuture<void> future = QtConcurrent::run(FilePlugin, &FileReadInterface::ReadCubeFromFile, FileName, m_cube);

            // Start the computation.
            futureWatcher.setFuture(future);

            // Display the dialog and start the event loop.
            dialog.exec();

            futureWatcher.waitForFinished();
            dialog.setValue(100);
            dialog.hide();


            timer.stop();

        }

        FilePlugin->DeleteData();
        
        IAttributes* attrCube = new Cube3DPluginAttributes(m_pluginsControl->GetPelengPlugins());

        if (m_pluginsControl->GetPelengPlugins().size() > 0)
        {


            PelengPluginLoader pelengLoader;
            m_pelengPlugins = pelengLoader.LoadPlugin("3DCube UI");
            m_pelengPlugins->Execute(cube, attrCube);


    }
   //m_pluginsControl->GetReadPlugins().first()->
}


void MainWindow::updateProgress()
{
   // TODO
    emit progressValueChanged(FilePlugin->getProgress());
}



bool MainWindow::loadFilePlugin()
{

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
     FilePlugin->cancel();
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


