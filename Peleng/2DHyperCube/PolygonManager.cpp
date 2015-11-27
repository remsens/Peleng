#include "PolygonManager.h"
#include "ui_PolygonManager.h"

PolygonManager::PolygonManager(int rows, int cols,
                               QCustomPlot *cusPlot, QWidget *parent2Dwindow) :
    QMainWindow(parent2Dwindow)
  , ui(new Ui::PolygonManager)
  , m_rows(rows)
  , m_cols (cols)
  , m_polyCreationInProcess(false)
  , m_flagDoubleClicked(false)
  , m_cusPlot(cusPlot)
  , m_parent2D(parent2Dwindow)

{
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableWidget->setSelectionMode()
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tableContextMenuRequest(QPoint)));
    connect(ui->tableWidget->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    connect(ui->buttonAddRegion,SIGNAL(clicked()),SLOT(onButtonAddRegion()));
    connect(ui->buttonRemoveRegion,SIGNAL(clicked()),SLOT(onButtonRemoveRegion()));
    connect(ui->buttonAddPolygon,SIGNAL(clicked()),SLOT(onButtonAddPolygon()));
}

void PolygonManager::createPolygonSlot()
{
    QString strForLineHelp = "Выберите точку; двойной щелчок для завершения";
    m_parent2D->setToolTip(strForLineHelp);
    m_cusPlot->setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));
    QPolygon polygon;
    m_polygonArr.append(polygon);
    m_polyCreationInProcess = true;
    connect(m_parent2D,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));

}

void PolygonManager::addPolygonPoint(uint x,uint y)
{
    QColor color = Qt::red;
    QPoint p(x,y);
    if (m_polygonArr.last().size() > 0)
        drawLine(m_polygonArr.last().last(), p,  color);
    m_polygonArr.last().append(p);
}

void PolygonManager::drawLine(QPoint p1, QPoint p2, QColor color)
{
    QCPItemLine *line = new QCPItemLine(m_cusPlot);
    line->start->setCoords(p1);
    line->end->setCoords(p2);
    line->setPen(QPen(color));
    m_cusPlot->addItem(line);
    m_cusPlot->replot();
}
void PolygonManager::mouseDblClickOnParentColorMap( QMouseEvent *)
{
    if (m_polyCreationInProcess)
        finishPolygonCreation();
}
void PolygonManager::finishPolygonCreation()
{
    disconnect(m_parent2D,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));
    QColor color = Qt::red;
    m_polyCreationInProcess = false;
    m_flagDoubleClicked = true;
    m_cusPlot->setCursor(QCursor(Qt::ArrowCursor));
    m_parent2D->setToolTip("");
    drawLine(m_polygonArr.last().last(), m_polygonArr.last().first(), color );
    QByteArray byteArr = byteMaskFromPolygons(m_polygonArr);
    QImage mask = imageFromByteMask(byteArr,color);
    drawImage(mask);
    m_cusPlot->replot();
}

QByteArray PolygonManager::byteMaskFromPolygons(QVector<QPolygon> polygonArr)
{
    QByteArray byteArr(m_rows*m_cols,0x00);
    for(int i = 0; i < m_rows; ++i)
    {
        for(int j = 0; j < m_cols; ++j)
        {
            foreach(QPolygon polygon, polygonArr)
            {
                if(polygon.containsPoint(QPoint(i,j),Qt::OddEvenFill))
                {
                    byteArr[i*m_cols+j] = 0x01;
                    //mask.setPixel(i,j,qRgba(255, 0, 0, 255));
                }
            }
        }
    }
    return byteArr;
}

void PolygonManager::saveByteMask(QByteArray byteArr)
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранить файл"),"*.area");
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(byteArr);
        file.close();
    }
}

QByteArray PolygonManager::loadByteMaskFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),"*.area");
    QFile file(fileName);
    QByteArray byteArr(m_rows*m_cols,0x00);
    if(file.open(QIODevice::ReadOnly))
    {
        if (file.size() != m_rows*m_cols) // потестировать это
        {
            qDebug()<< "file.size() != m_rows*m_cols;  чтение запрещено";
            return byteArr; //возвращаем заполненный нулями байтэррэй
        }
        else
        {
            byteArr = file.readAll();
            file.close();
            return byteArr;
        }
    }
    else
    {
        qDebug()<<fileName<<" isn't opened";
        return byteArr;
    }
}


QImage PolygonManager::imageFromByteMask(QByteArray byteArr, QColor color)
{
    QImage mask(m_rows,m_cols,QImage::Format_ARGB32);
    mask.fill(qRgba(0, 0, 0, 0));
    for(int i = 0; i < m_rows; ++i)
    {
        for(int j = 0; j < m_cols; ++j)
        {
            if(byteArr[i*m_cols+j] == 0x01)
                mask.setPixel(i,j,color.rgba());
        }
    }
    return mask;
    //drawImage(mask);

}
void PolygonManager::drawImage(QImage mask)
{
    QCPItemPixmap *pixItem = new QCPItemPixmap(m_cusPlot);
    QPixmap alphaImage(QPixmap::fromImage(mask));
    pixItem->setPixmap(alphaImage);
    pixItem->setScaled(true);
    m_cusPlot->addItem(pixItem);
    pixItem->topLeft->setCoords(0,0);
    pixItem->bottomRight->setCoords(m_rows-1,m_cols-1);
    pixItem->setClipToAxisRect(true);
    pixItem->setClipAxisRect(m_cusPlot->axisRect());
}

void PolygonManager::tableContextMenuRequest(QPoint pos)
{

    QModelIndex index = ui->tableWidget->indexAt(pos);
    if(index.isValid())
    {
        QMenu *menu = new QMenu(this);
        QAction* actionColor = new QAction("Выбор цвета",this);
        QAction* action2 = new QAction("test action 2",this);
        QAction* action3 = new QAction("test action 3",this);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->addAction(actionColor);
        menu->addAction(action2);
        menu->addAction(action3);
        menu->popup(QCursor::pos());//ui->tableWidget->mapToGlobal(pos)
        connect(actionColor,SIGNAL(triggered()),SLOT(pickColor()));
        //QColorDialog
    }

}
void PolygonManager::pickColor() // должен возвращать цвет
{
    QColor color = QColorDialog::getColor(Qt::white);
    ui->tableWidget->selectedItems().at(1)->setBackgroundColor(color);

}

void PolygonManager::onButtonAddRegion()
{
    Region region;
    m_RegionArr.append(region);

    int rowsCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowsCount);
    ui->tableWidget->setItem(rowsCount,0, new QTableWidgetItem());
    ui->tableWidget->setItem(rowsCount,1, new QTableWidgetItem());
}

void PolygonManager::onButtonRemoveRegion()
{
    if( ui->tableWidget->selectedItems().isEmpty())
        return;
    ui->tableWidget->removeRow(ui->tableWidget->selectedItems().at(0)->row());
}

void PolygonManager::onButtonAddPolygon()
{
    createPolygonSlot();
}

void PolygonManager::currentRowChanged(QModelIndex i1, QModelIndex i2)
{

}

PolygonManager::~PolygonManager()
{
    delete ui;
}
