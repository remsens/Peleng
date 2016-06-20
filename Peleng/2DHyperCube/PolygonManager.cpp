#include "PolygonManager.h"
#include "ui_PolygonManager.h"
#include "../Library/GenericExc.h"
#include "../Library/Spectr.h"
#include "../Library/structures.h"
#include <QTextStream>
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
    PolygonObject polygonArea;
    m_RegionArr[m_currIndexRegion].m_polygonObjects.append(polygonArea);
    m_polyCreationInProcess = true;
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
    disconnect(m_parent2D,SIGNAL(signalDoubleCordsClicked(double,double)),this,SLOT(addPolygonPoint(double,double)));
    QColor color = m_RegionArr.at(m_currIndexRegion).m_color;
    m_polyCreationInProcess = false;
    m_flagDoubleClicked = true;
    m_cusPlot->setCursor(QCursor(Qt::ArrowCursor));
    m_parent2D->setToolTip("");
    drawLine(m_RegionArr.at(m_currIndexRegion).m_polygonObjects.last().ijVertices.last(),
             m_RegionArr.at(m_currIndexRegion).m_polygonObjects.last().ijVertices.first(), color );

    for (int i = 0; i < m_RegionArr[m_currIndexRegion].m_polygonObjects.count(); ++i){
        if( !m_RegionArr[m_currIndexRegion].m_polygonObjects[i].lines.isEmpty()){
            foreach (QCPItemLine* line, m_RegionArr[m_currIndexRegion].m_polygonObjects[i].lines) {
                m_cusPlot->removeItem(line);
            }
            m_RegionArr[m_currIndexRegion].m_polygonObjects[i].lines.clear();
        }
    }

    drawregion(m_RegionArr[m_currIndexRegion]);
    calcPolygonBLcord(m_RegionArr[m_currIndexRegion]);
    m_cusPlot->replot();
}

void PolygonManager::resizedHyperCube()
{
    m_rows = m_cube->GetLines();
    m_cols = m_cube->GetColumns();
    int initIndex = m_currIndexRegion;
    int i = 0;
    for(Region &region : m_RegionArr){

            for (int i = 0; i < region.m_polygonObjects.count(); ++i){
                if( !region.m_polygonObjects[i].lines.isEmpty()){
                    foreach (QCPItemLine* line, region.m_polygonObjects[i].lines) {
                        m_cusPlot->removeItem(line);
                    }
                }
            }
            m_cusPlot->replot();
            m_currIndexRegion = i;
            create_ijVerticesFromBLdegVertices(region);
            drawregion(region);

        i++;
    }
    m_currIndexRegion = initIndex;
}


void PolygonManager::saveRegionToXML(Region region, QString fileName)
{
    QTextCodec *russianCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(russianCodec);
    QDomDocument doc("region_of_interest");
    QDomElement rootElement = doc.createElement("region");
    rootElement.setAttribute("name", region.m_name);
    rootElement.setAttribute("color", region.m_color.name());
    foreach(PolygonObject polygon, region.m_polygonObjects)
    {
        QDomElement elPolygon = doc.createElement("polygon");
        foreach (QPointF vertex, polygon.BLdegVertices)
        {
            QDomElement elPoint = doc.createElement("point");
            elPoint.setAttribute("breadth",vertex.x());
            elPoint.setAttribute("longitude ",vertex.y());
            elPolygon.appendChild(elPoint);
        }
        rootElement.appendChild(elPolygon);
    }
    doc.appendChild(rootElement);
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) <<  doc.toString();
        file.close();
    }
}

void PolygonManager::calcPolygonBLcord(Region &region)
{
    for (PolygonObject &polygon : region.m_polygonObjects) {
        polygon.BLdegVertices.clear();
        for(QPointF &pIm : polygon.ijVertices){
            point pBL = m_cube->getBLdegreeCords(pIm.x(),pIm.y());
            QPointF newBLp(pBL.x,pBL.y);
            polygon.BLdegVertices.append(newBLp);
        }
    }
}

Region PolygonManager::loadRegionFromXML(QString fileName)
{
    QFile file(fileName);
    Region region;
    if(!file.open(QIODevice::ReadOnly)){
        file.close();
        QMessageBox::critical(this,"ошибка","ЭНевозможно открыть файл");
        return region;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::critical(this, "Ошибка добавления действия", "Невозможно установить контент файла");
    }
    //считываем все данные из xml в region
    QDomNode rootNode = doc.namedItem("region");
    region.m_color.setNamedColor( rootNode.toElement().attribute("color",""));
    region.m_name = rootNode.toElement().attribute("name","");
    QDomNodeList poligons = rootNode.childNodes();
    int iPoly = 0;
    for(int i = 0; i < poligons.count(); i++){
        if(poligons.at(i).nodeName() == "polygon"){
            PolygonObject polObj;
            region.m_polygonObjects.append(polObj);
            QDomNodeList points = poligons.at(iPoly).childNodes();
            int iPoint = 0;
            for(int j = 0; j < points.count(); j++){

                if(points.at(j).nodeName() == "point"){

                    QPointF BLpoint(points.at(iPoint).toElement().attribute("breadth").toDouble(),
                                    points.at(iPoint).toElement().attribute("longitude").toDouble());
                    region.m_polygonObjects.last().BLdegVertices.append(BLpoint);
                    iPoint += 1;
                }
            }
            iPoly +=1;
        }
    }
    //переводим координаты в пиксельные
    //pointInt ijP = getImageCordsFromBLdeg()
    return region;
}

void PolygonManager::create_ijVerticesFromBLdegVertices(Region &region)
{
    for(PolygonObject &polObj : region.m_polygonObjects){
        polObj.ijVertices.clear();
        QPolygonF ijVertices(polObj.BLdegVertices.count());
        for(int i = 0; i < ijVertices.count(); ++i){
            pointInt ijCord = m_cube->getImageCordsFromBLdeg(polObj.BLdegVertices.at(i).x(),
                                                             polObj.BLdegVertices.at(i).y());
            ijVertices[i].setX(ijCord.x);
            ijVertices[i].setY(ijCord.y);
        }
        polObj.ijVertices.append(ijVertices);
    }
}

bool PolygonManager::is_ijInRegion(int i, int j, Region region)
{
    foreach(PolygonObject polygon, region.m_polygonObjects)
    {
        if(polygon.ijVertices.containsPoint(QPoint(i,j),Qt::WindingFill))
        {
            return true;
        }
    }
    return false;
}

void PolygonManager::drawregion(Region &region)
{
    for(auto &polObj : region.m_polygonObjects){
        QColor color = region.m_color;
        drawPoligonObject(polObj, color);
    }
}

void PolygonManager::drawPoligonObject(PolygonObject &polObj, QColor color)
{
    if(polObj.contour != NULL)
        m_cusPlot->removePlottable(polObj.contour);
    polObj.contour = drawPoligon(polObj.ijVertices, color);
}

QCPCurve *PolygonManager::drawPoligon(QPolygonF polygon, QColor color)
{
    QCPCurve *contour = new QCPCurve(m_cusPlot->xAxis, m_cusPlot->yAxis);
    color.setAlphaF(0.7);
    contour->setBrush(QBrush(color));
    contour->setPen(QPen(color));
    m_cusPlot->addPlottable(contour);
    QVector<double> key;
    QVector<double> val;
    keyValueFromPoligon(key, val, polygon);
    contour->setData(key,val);
    m_cusPlot->replot();
    return contour;
}

void PolygonManager::keyValueFromPoligon(QVector<double> &key, QVector<double> &value, QPolygonF polygon)
{
    key.clear();
    value.clear();
    for(QPointF p : polygon){
        key.append(p.x());
        value.append(p.y());
    }
    //соединяем линией первую и последную точку
    key.append(polygon.first().x());
    value.append(polygon.first().y());
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
    Region region;
    m_RegionArr.append(region);
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


    for (int i = 0; i < m_RegionArr[selectedRow].m_polygonObjects.length(); ++i){
        if(m_RegionArr[selectedRow].m_polygonObjects[i].contour != NULL)
            m_cusPlot->removePlottable(m_RegionArr[selectedRow].m_polygonObjects[i].contour);
        foreach (QCPItemLine* line, m_RegionArr[selectedRow].m_polygonObjects[i].lines) {
            m_cusPlot->removeItem(line);
        }
    }
    m_cusPlot->replot();

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

    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),"*.xml");
    Region newReg = loadRegionFromXML(fileName);
    create_ijVerticesFromBLdegVertices(newReg);
    m_RegionArr.append(newReg);
    int rowsCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowsCount);
    ui->tableWidget->setItem(rowsCount,0, new QTableWidgetItem());
    ui->tableWidget->setItem(rowsCount,1, new QTableWidgetItem());
    ui->tableWidget->item(rowsCount,1)->setBackgroundColor(newReg.m_color);
    ui->tableWidget->item(rowsCount,0)->setText(newReg.m_name);
    ui->tableWidget->setCurrentCell(ui->tableWidget->rowCount() - 1,0);

    drawregion(m_RegionArr.last());

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
            if( is_ijInRegion(i,j,m_RegionArr.at(m_currIndexRegion)))//m_RegionArr.at(m_currIndexRegion).m_polygonObjects.m_byteArr.at(i*m_cols+j) == 0x01
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
            if(is_ijInRegion(i,j,m_RegionArr.at(m_currIndexRegion)))
            {
                ijArr.append(QPoint(i,j));
            }
        }
    }
    for (int k = 0; k < Chnls; ++k)
    {
        foreach(QPoint p,ijArr)
            buf.append(m_cube->GetDataPoint(p.x(),p.y(),k)); //  buf.append(data[k][p.x()*m_cols + p.y()]);
        std[k] = sqrt(calcVariance(buf));
        buf.clear();
    }
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
    m_attributes->GetAvailablePlugins().value("Spectr UI")->Execute(m_cube, m_attributes);
}

double PolygonManager::calcVariance(QVector<double> X)
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
