#include "PolygonManager.h"
#include "ui_PolygonManager.h"
#include "../Library/GenericExc.h"
#include "../Library/Spectr.h"
#include "../Library/structures.h"
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class BackgroundDelegate : public QStyledItemDelegate // для того, чтобы через выделение не был виден цвет ячейки
{
public:
    explicit BackgroundDelegate(QObject *parent = 0)
        : QStyledItemDelegate(parent)
    {
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const
    {
        QVariant background = index.data(Qt::BackgroundRole);
        if (background.canConvert<QBrush>())
            painter->fillRect(option.rect, background.value<QBrush>());

        QStyledItemDelegate::paint(painter, option, index);

        if(option.state & QStyle::State_Selected)
        {
            painter->save();
            QPen pen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
            int w = pen.width()/2;
            painter->setPen(pen);
            painter->drawRect(option.rect.adjusted(w,w,-w,-w));
            painter->restore();
        }
    }
};

PolygonManager::PolygonManager(HyperCube *cube, Attributes *attributes,
                               QCustomPlot *cusPlot, QWidget *parent2Dwindow) :
    QMainWindow(0)
  , ui(new Ui::PolygonManager)
  , m_cube(cube)
  , m_attributes(attributes)
  , m_rows(cube->GetLines())
  , m_cols (cube->GetColumns())
  , m_polyCreationInProcess(false)
  , m_flagDoubleClicked(false)
  , m_cusPlot(cusPlot)
  , m_parent2D(parent2Dwindow)
  , m_currIndexRegion(-1)

{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons2Dcube/icons/polygon.png"));
    ui->tableWidget->setItemDelegate(new BackgroundDelegate(this));
    ui->tableWidget->setStyleSheet("selection-background-color: rgba(128, 128, 128, 40);");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setStyleSheet("selection-background-color: rgba(0, 0, 128, 100);");
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tableContextMenuRequest(QPoint)));
    connect(ui->tableWidget->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    connect( ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(itemChanged(QTableWidgetItem*)));
    connect(ui->buttonAddRegion,SIGNAL(clicked()),SLOT(onButtonAddRegion()));
    connect(ui->buttonRemoveRegion,SIGNAL(clicked()),SLOT(onButtonRemoveRegion()));
    connect(ui->buttonAddPolygon,SIGNAL(clicked()),SLOT(onButtonAddPolygon()));
    connect(ui->buttonSaveRegion,SIGNAL(clicked()),SLOT(onButtonSaveRegion()));
    connect(ui->buttonLoadregion,SIGNAL(clicked()),SLOT(onButtonLoadRegion()));
}

void PolygonManager::createPolygonSlot()
{
    if (m_RegionArr.isEmpty() || ui->tableWidget->selectedItems().isEmpty())
        return;
    QString strForLineHelp = "Выберите точку; двойной щелчок для завершения";
    m_parent2D->setToolTip(strForLineHelp);
    m_cusPlot->setCursor(QCursor(QPixmap(":/IconsCube/iconsCube/start_flag.png"),10,29));
    QPolygonF polygon;
    PolygonObject polygonArea;
    m_RegionArr[m_currIndexRegion].m_polygonObjects.append(polygonArea);
    m_polyCreationInProcess = true;
    //connect(m_parent2D,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));
    connect(m_parent2D,SIGNAL(signalDoubleCordsClicked(double,double)),this,SLOT(addPolygonPoint(double,double)));
}

void PolygonManager::addPolygonPoint(double x,double y)
{
    QColor color = m_RegionArr.at(m_currIndexRegion).m_color;
    QPointF p(x,y);
    if (m_RegionArr.at(m_currIndexRegion).m_polygonObjects.last().ijVertices.size() > 0)
        drawLine(m_RegionArr.at(m_currIndexRegion).m_polygonObjects.last().ijVertices.last(), p,  color);
    m_RegionArr[m_currIndexRegion].m_polygonObjects.last().ijVertices.append(p);
}

void PolygonManager::drawLine(QPointF p1, QPointF p2, QColor color)
{
    QCPItemLine *line = new QCPItemLine(m_cusPlot);
//    m_RegionArr[m_currIndexRegion].m_lines.append(line);
    m_RegionArr[m_currIndexRegion].m_polygonObjects.last().lines.append(line);
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
//    disconnect(m_parent2D,SIGNAL(signalCurrentDataXY(uint,uint)),this,SLOT(addPolygonPoint(uint,uint)));
    disconnect(m_parent2D,SIGNAL(signalDoubleCordsClicked(double,double)),this,SLOT(addPolygonPoint(double,double)));
    QColor color = m_RegionArr.at(m_currIndexRegion).m_color;
    m_polyCreationInProcess = false;
    m_flagDoubleClicked = true;
    m_cusPlot->setCursor(QCursor(Qt::ArrowCursor));
    m_parent2D->setToolTip("");
    drawLine(m_RegionArr.at(m_currIndexRegion).m_polygonObjects.last().ijVertices.last(),
             m_RegionArr.at(m_currIndexRegion).m_polygonObjects.last().ijVertices.first(), color );
    QByteArray byteArr = byteMaskFromPolygons(m_RegionArr.at(m_currIndexRegion).m_polygonObjects); //байтовая маска из полигонов
    m_RegionArr[m_currIndexRegion].m_byteArr = byteMaskFrom2ByteMasks(
                m_RegionArr[m_currIndexRegion].m_byteArr, byteArr);//добавляем полученную маску к уже существующей маске
    QImage mask = imageFromByteMask(m_RegionArr[m_currIndexRegion].m_byteArr,color);
    drawImage(mask);
    calcPolygonBLcord(m_RegionArr[m_currIndexRegion]);
    m_cusPlot->replot();
}

QByteArray PolygonManager::byteMaskFromPolygons(QVector<PolygonObject> polygonArr)
{
    QByteArray byteArr(m_rows*m_cols,0x00);
    for(int i = 0; i < m_rows; ++i)
    {
        for(int j = 0; j < m_cols; ++j)
        {
            foreach(PolygonObject polygon, polygonArr)
            {
                if(polygon.ijVertices.containsPoint(QPoint(i,j),Qt::WindingFill))
                {
                    byteArr[i*m_cols+j] = 0x01;
                }
            }
        }
    }
    return byteArr;
}

QByteArray PolygonManager::byteMaskFrom2ByteMasks(QByteArray arr1, QByteArray arr2)
{
    QByteArray byteArr(m_rows*m_cols,0x00);
    if(arr1.size() != byteArr.size()  ||  arr2.size() != byteArr.size())
    {
        qDebug()<<"in func byteMaskFrom2ByteMasks arr1 and arr2 sizes are DIFFERENT or != m_rows*m_cols";
        return byteArr;
    }
    for(int i = 0; i < m_rows; ++i)
    {
        for(int j = 0; j < m_cols; ++j)
        {
            if(arr1.at(i*m_cols+j) == 0x01  ||  arr2.at(i*m_cols+j) == 0x01)
                byteArr[i*m_cols+j] = 0x01;
        }
    }
    return byteArr;
}

void PolygonManager::saveByteMask(QByteArray byteArr, QString fileName)
{

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(byteArr);
        file.close();
    }
}

void PolygonManager::saveRegionToXML(Region region, QString fileName)
{
    QFile file(fileName);
    QDomDocument doc("Region_of_interest");
    QDomElement rootElement = doc.createElement("region");
    rootElement.setAttribute("name", "Лес");
    rootElement.setAttribute("color", "#FFFFFF");
    QDomElement polygon1 = doc.createElement("polygon");
    QDomElement point1 = doc.createElement("point");
    point1.setAttribute("breadth",34.435);
    point1.setAttribute("longitude ",84.4235);
    polygon1.appendChild(point1);


    rootElement.appendChild(polygon1);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) <<  doc.toString();
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
            throw GenericExc("не подходит размер выбранной маски к размеру изображения",-1);
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
            if(byteArr.at(i*m_cols+j) == 0x01)
            {
                mask.setPixel(i,j,QColor(color.red(), color.green(), color.blue(), 120).rgba());
            }
        }
    }
    return mask;
}
void PolygonManager::drawImage(QImage mask)
{
    if(m_RegionArr[m_currIndexRegion].m_pixItem != NULL)
        m_cusPlot->removeItem(m_RegionArr[m_currIndexRegion].m_pixItem);

    QCPItemPixmap *pixItem = new QCPItemPixmap(m_cusPlot);
    QPixmap alphaImage(QPixmap::fromImage(mask));
    pixItem->setPixmap(alphaImage);
    pixItem->setScaled(true,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    m_cusPlot->addItem(pixItem);
    pixItem->topLeft->setType(QCPItemPosition::ptPlotCoords );
    pixItem->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    pixItem->topLeft->setCoords(-0.5,-0.5);
    pixItem->bottomRight->setCoords(m_rows-1+0.5,m_cols-1+0.5);
    pixItem->setClipToAxisRect(true);
    pixItem->setClipAxisRect(m_cusPlot->axisRect());

    m_RegionArr[m_currIndexRegion].m_pixItem = pixItem;
}

void PolygonManager::calcPolygonBLcord(Region &region)
{
    foreach (PolygonObject polygon , region.m_polygonObjects) {
        foreach(QPointF pIm,polygon.ijVertices){
            polygon.ijVertices.clear();
            point pBL = m_cube->getBLdegreeCords(pIm.x(),pIm.y());
            QPointF newBLp(pBL.x,pBL.y);
            polygon.BLdegVertices.append(newBLp);
        }
    }
}

void PolygonManager::tableContextMenuRequest(QPoint pos)
{

    QModelIndex index = ui->tableWidget->indexAt(pos);
    if(index.isValid())
    {
        QMenu *menu = new QMenu(this);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->popup(QCursor::pos());
        menu->addAction("Выбор цвета",this,SLOT(pickColor()));
        menu->addAction(QIcon(":/iconsPolyManager/icons/average.png"),"Средний спектр",this,SLOT(onMenuAverageSpectr()));
        menu->addAction(QIcon(":/iconsPolyManager/icons/average.png"),"СКО",this,SLOT(onMenuStandardDeviation()));
    }

}
void PolygonManager::pickColor()
{
    QColor color = QColorDialog::getColor(Qt::white);
    ui->tableWidget->selectedItems().at(1)->setBackgroundColor(color);
    m_RegionArr[m_currIndexRegion].m_color = color;

}

void PolygonManager::onButtonAddRegion()
{
    QByteArray byteArr(m_rows*m_cols,0x00);
    Region region;
    m_RegionArr.append(region);
    m_RegionArr.last().m_byteArr = byteArr;
    int rowsCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowsCount);
    ui->tableWidget->setItem(rowsCount,0, new QTableWidgetItem());
    ui->tableWidget->setItem(rowsCount,1, new QTableWidgetItem());
    ui->tableWidget->item(rowsCount,1)->setBackgroundColor(region.m_color);
    ui->tableWidget->item(rowsCount,0)->setText(m_RegionArr.last().m_name);

}

void PolygonManager::onButtonRemoveRegion()
{
    if( ui->tableWidget->selectedItems().isEmpty())
        return;
    int selectedRow = ui->tableWidget->selectedItems().at(0)->row();
    if(m_RegionArr[selectedRow].m_pixItem != NULL)
    {
        m_cusPlot->removeItem(m_RegionArr[selectedRow].m_pixItem);
        for (int i = 0; i < m_RegionArr[selectedRow].m_polygonObjects.length(); ++i){
            foreach (QCPItemLine* line, m_RegionArr[selectedRow].m_polygonObjects[i].lines) {
                m_cusPlot->removeItem(line);
            }
        }
        m_cusPlot->replot();
    }
    ui->tableWidget->removeRow(selectedRow);
    m_RegionArr.remove(selectedRow);
    m_currIndexRegion = selectedRow;
}

void PolygonManager::onButtonAddPolygon()
{
    createPolygonSlot();
}

void PolygonManager::onButtonSaveRegion()
{
    if (ui->tableWidget->selectedItems().isEmpty())
        return;
    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранить файл"),m_RegionArr.at(m_currIndexRegion).m_name,"*.xml");
    if (fileName.contains(".area",Qt::CaseInsensitive))
        saveRegionToXML(m_RegionArr.at(m_currIndexRegion),fileName);
    else
        saveRegionToXML(m_RegionArr.at(m_currIndexRegion),fileName+".xml");
}

void PolygonManager::onButtonLoadRegion()
{
//    if (ui->tableWidget->selectedItems().isEmpty())
//        return;
    onButtonAddRegion(); //создаем новый регион интереса, в который загружаем данные с диска
    ui->tableWidget->setCurrentCell(ui->tableWidget->rowCount() - 1,0);
    try
    {
        QByteArray byteArr = loadByteMaskFromFile();
        QColor color = m_RegionArr.at(m_currIndexRegion).m_color;
        QImage mask = imageFromByteMask(byteArr,color);
        drawImage(mask);
        m_cusPlot->replot();
    }
    catch(GenericExc e)
    {
        QMessageBox::warning(this,"предупреждение","не подходит размер выбранной маски к размеру изображения");
        qDebug()<<e.GetWhat();
    }

}

void PolygonManager::itemChanged(QTableWidgetItem *it)
{

    if (it->column() == 0)
        m_RegionArr[it->row()].m_name = it->text();

}

void PolygonManager::onMenuAverageSpectr()
{
    //1.область как маска
    //2.считаем средний спектр по выбранной маске

    int Chnls = m_cube->GetCountofChannels();
    int numSpctrs = 0;
    double* spectrSum = new double[Chnls];
    for (int i = 0; i < Chnls; ++i)
    {
        spectrSum[i] = 0;
    }
    for(int i = 0; i < m_rows; ++i)
    {
        for(int j = 0; j < m_cols; ++j)
        {
            if(m_RegionArr[m_currIndexRegion].m_byteArr.at(i*m_cols+j) == 0x01)
            {
                numSpctrs++;
                QVector<double> spectr;
                m_cube->GetSpectrumPoint(i, j, spectr); // записали в pSpectrValues из гиперкуба
                for (int k = 0; k < Chnls; ++k)
                {
                    spectrSum[k] += spectr.at(k);
                }
            }
        }
    }
    std::vector<double> vector(spectrSum, spectrSum + Chnls);
    delete[] spectrSum;
    QVector<double> averageSpectrVect = QVector<double>::fromStdVector(vector);
    for(int i = 0; i < Chnls; ++i)
        averageSpectrVect[i] /= numSpctrs;
    QList<double> Wawes;
    Wawes.append(m_cube->GetListOfChannels());
    QVector<double> wawesVect;
    for (int i = 0; i < Chnls; ++i )
       wawesVect.push_back(Wawes[i]);
    // править размерность
    QString title = "Среднее по региону '" + m_RegionArr[m_currIndexRegion].m_name + "'";
    Spectr* spectr = new Spectr (m_cube, wawesVect, averageSpectrVect, title, Measurements::ADC);
    m_attributes->SetCurrentXUnits(wawesVect);
    m_attributes->SetCurrentYUnits(averageSpectrVect);
    m_attributes->SetExternalSpectrFlag(true);
    m_attributes->SetCurrentSpectr(spectr);
    //m_attributes->SetCurrentTitle("Среднее по региону '" + m_RegionArr[m_currIndexRegion].m_name + "'");
    m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(m_cube, m_attributes);
}

void PolygonManager::onMenuStandardDeviation()
{
    int Chnls = m_cube->GetCountofChannels();
    QVector<double> buf;
    QVector<double> std(Chnls,0); //СКО
    QVector<QPoint> ijArr;
    for(int i = 0; i < m_rows; ++i) // долго из-за порядка циклов
    {
        for(int j = 0; j < m_cols; ++j)
        {
            if(m_RegionArr[m_currIndexRegion].m_byteArr.at(i*m_cols+j) == 0x01)
            {
                ijArr.append(QPoint(i,j));
            }
        }
    }
    for (int k = 0; k < Chnls; ++k)
    {
        foreach(QPoint p,ijArr)
             buf.append(m_cube->GetDataPoint(p.x(),p.y(),k)); //  buf.append(data[k][p.x()*m_cols + p.y()]);
        std[k] = sqrt(calcStandardDeviation(buf));
        buf.clear();
    }
//    for (int k = 0; k < Chnls; ++k)
//    {
//        for(int i = 0; i < m_rows; ++i) // долго из-за порядка циклов
//        {
//            for(int j = 0; j < m_cols; ++j)
//            {
//                if(m_RegionArr[m_currIndexRegion].m_byteArr[i*m_cols+j] == 0x01)
//                {
//                   buf.append(data[k][i*m_cols + j]);
//                }
//            }
//        }
//        std[k] = sqrt(calcStandardDeviation(buf));
//        buf.clear();
//    }


    QList<double> Wawes;
    Wawes.append(m_cube->GetListOfChannels());
    QVector<double> wawesVect;
    for (int i = 0; i < Chnls; ++i )
       wawesVect.push_back(Wawes[i]);
    QString title = "СКО по региону " + m_RegionArr[m_currIndexRegion].m_name;
    //править
    Spectr* spectr = new Spectr (m_cube, wawesVect, std, title, Measurements::ADC);
    m_attributes->SetCurrentXUnits(wawesVect);
    m_attributes->SetCurrentYUnits(std);
    m_attributes->SetExternalSpectrFlag(true);
    m_attributes->SetCurrentSpectr(spectr);

   /* m_attributes->SetXUnit(wawesVect);
    m_attributes->SetYUnit(std);
    m_attributes->SetExternalSpectrFlag(true);
    QList<Attributes::DescriptionSpectr> list;
    Attributes::DescriptionSpectr descriptionAverSpctr;
    descriptionAverSpctr.title = "СКО по региону '";
    descriptionAverSpctr.description= m_RegionArr[m_currIndexRegion].m_name + "'";
    list.append(descriptionAverSpctr);
    m_attributes->SetDescriptionSpectr(list);*/

    m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(m_cube, m_attributes);
}

double PolygonManager::calcStandardDeviation(QVector<double> X)
{
    double mean=0.0, std=0.0;
    int n = X.length();
    for(int i=0; i<n;++i)
    {
        mean += X.at(i);
    }
    mean = mean/n;
    for(int i=0; i<n;++i)
        std += (X.at(i)-mean)*(X.at(i)-mean);

    return std / n;
}

void PolygonManager::currentRowChanged(QModelIndex curr, QModelIndex prev)
{
    if(curr.row() != -1)
    {
        ui->buttonAddPolygon->setEnabled(true);
        ui->buttonSaveRegion->setEnabled(true);
        ui->buttonRemoveRegion->setEnabled(true);
    }
    else
    {
        ui->buttonAddPolygon->setEnabled(false);
        ui->buttonSaveRegion->setEnabled(false);
        // дисконнект/коннект нужен чтобы исправить баг с вызовом currentRowChanged при скрытии кнопки
        disconnect(ui->tableWidget->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(currentRowChanged(QModelIndex,QModelIndex)));
        ui->buttonRemoveRegion->setEnabled(false);
        connect(ui->tableWidget->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    }
    m_currIndexRegion = curr.row();
}

PolygonManager::~PolygonManager()
{
    delete ui;
}
