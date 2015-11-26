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
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0, new QTableWidgetItem("first"));
    ui->tableWidget->setItem(0,1, new QTableWidgetItem("second"));
    ui->tableWidget->itemAt(0,0)->setBackgroundColor(Qt::red);
    ui->tableWidget->insertRow(1);

   ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tableContextMenuRequest(QPoint)));
   connect(ui->tableWidget->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(currentRowChanged(QModelIndex,QModelIndex)));

}

void PolygonManager::createPolygonSlot()
{
    QString strForLineHelp = "Выберите точку; двойной щелчок для завершения";
    m_parent2D->setToolTip(strForLineHelp);
    m_parent2D->setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));
    QPolygon polygon;
    polygonArr.append(polygon);
    m_polyCreationInProcess = true;
    connect(m_parent2D,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));

}

void PolygonManager::addPolygonPoint(uint x,uint y)
{
    QColor color = Qt::red;
    QPoint p(x,y);
    if (polygonArr.last().size() > 0)
        drawLine(polygonArr.last().last(), p,  color);
    polygonArr.last().append(p);
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

void PolygonManager::finishPolygonCreation()
{
    disconnect(m_parent2D,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));
    QColor color = Qt::red;
    m_polyCreationInProcess = false;
    m_flagDoubleClicked = true;
    m_parent2D->setCursor(QCursor(Qt::ArrowCursor));
    m_parent2D->setToolTip("");
    drawLine(polygonArr.last().last(), polygonArr.last().first(), color );
    //QImage mask = maskFromPolygons(polygonArr);//дописать
    QCPItemPixmap *pixItem = new QCPItemPixmap(m_cusPlot);
    // QPixmap alphaImage(QPixmap::fromImage(mask));
    //pixItem->setPixmap(alphaImage);
    pixItem->setScaled(true,Qt::KeepAspectRatio,Qt::FastTransformation);
    m_cusPlot->addItem(pixItem);
    pixItem->topLeft->setCoords(0,0);
    pixItem->bottomRight->setCoords(m_rows-1,m_cols-1);
    pixItem->setClipToAxisRect(true);
    pixItem->setClipAxisRect(m_cusPlot->axisRect());
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
            if(byteArr[i*m_cols+j] == (char)0x01) //проверить (char)
                mask.setPixel(i,j,color.rgba());
        }
    }
    return mask;
    drawImage(mask);

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
    QMenu *menu = new QMenu(this);
    QAction* action = new QAction("test action",this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->addAction(action);
    //ui->tableWidget->selectro
    menu->popup(ui->tableWidget->mapToGlobal(pos));
}

void PolygonManager::currentRowChanged(QModelIndex i1, QModelIndex i2)
{

}

PolygonManager::~PolygonManager()
{
    delete ui;
}
