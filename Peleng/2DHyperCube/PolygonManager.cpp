#include "PolygonManager.h"
#include "ui_PolygonManager.h"

PolygonManager::PolygonManager(int rows, int cols,
                               QCustomPlot *cusPlot, QWidget *parent2Dwindow) :
    QMainWindow(parent2Dwindow)
    , m_rows(rows)
    , m_cols (cols)
    , m_polyCreationInProcess(false)
    , m_flagDoubleClicked(false)
    , m_cusPlot(cusPlot)
    , m_parent2D(parent2Dwindow)
    , ui(new Ui::PolygonManager)
{
    ui->setupUi(this);
    //m_parent2D = dynamic_cast<Main2DWindow*>(parent2Dwindow);
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0, new QTableWidgetItem("first"));
    ui->tableWidget->setItem(0,1, new QTableWidgetItem("second"));
    ui->tableWidget->itemAt(0,0)->setBackgroundColor(Qt::red);
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
void PolygonManager::drawLine(QPoint p1, QPoint p2, QColor color)
{
    QCPItemLine *line = new QCPItemLine(m_cusPlot);
    line->start->setCoords(p1);
    line->end->setCoords(p2);
    line->setPen(QPen(color));
    m_cusPlot->addItem(line);
    m_cusPlot->replot();
}

PolygonManager::~PolygonManager()
{

    delete ui;
}
