#ifndef POLYGONMANAGER_H
#define POLYGONMANAGER_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "Region.h"
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"
namespace Ui {
class PolygonManager;
}

class PolygonManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit PolygonManager(HyperCube *cube,Attributes* attributes,
                            QCustomPlot *cusPlot, QWidget *parent2Dwindow);
    ~PolygonManager();
    //! функция,завершающая создание полигона
    void finishPolygonCreation();
    void setRows(int rows){m_rows = rows;}
    void setCols(int cols){m_cols = cols;}
    void resizedHyperCube();
public slots:
    //! слот соединяющийся с сигналом по экшену в контестном меню с 2D куба
    void createPolygonSlot();

    //! слот обрабатывающий двойной клик по колормэпу 2D куба
    void mouseDblClickOnParentColorMap( QMouseEvent *);
private:
    //! функция отрисовки линии на customPlot
    void drawLine(QPointF p1, QPointF p2, QColor color);

    //! функция сохранения региона интереса на диск
    void saveRegionToXML(Region region, QString fileName);

    //! функция создания изображения(с альфа-каналом) из региона
    QImage imageFromRegion(Region region);

    //! функция, рисующая изображение текущего (т.е. с индексом m_currIndexRegion) региона
    void drawImage(QImage mask);

    //! функция рассчета широты/долготы в градусах для вершин полигона
    //! рассчитаные координаты записываются в region.m_polygonObjects.BLdegVertices
    void calcPolygonBLcord(Region& region);

    //! функция загрузки региона с диска из XML-файла
    Region loadRegionFromXML(QString fileName);

    //! функция для рассчета пиксельных координат вершин полигонов региона по широте и долготе вершин полигонов в регионе
    void create_ijVerticesFromBLdegVertices(Region& region);

    //! функция для определения, попадает ли пиксель изображения в выбранный регион
    bool is_ijInRegion(int i, int j, Region region);

private slots:


    //! слот добавления точки полигона по клику
    void addPolygonPoint(double x, double y);

    //! слот обработки контекстного tableWidgetа
    void tableContextMenuRequest(QPoint pos);

    //! слот обработки изменения выделения в tableWidget
    void currentRowChanged(QModelIndex curr, QModelIndex prev);

    //! слот выбора цвета
    void pickColor();

    //! слот обработчика кнопки "Добавить регион интереса"
    void onButtonAddRegion();

    //! слот обработчика кнопки "Удалить регион интереса"
    void onButtonRemoveRegion();

    //!  слот обработчика кнопки "Добавить полигон"
    void onButtonAddPolygon();

    //!  слот обработчика кнопки "Сохранить область"
    void onButtonSaveRegion();

    //!  слот обработчика кнопки "Загрузить область"
    void onButtonLoadRegion();

    //!  слот обработки изменения item'а листвиджета.
    void itemChanged(QTableWidgetItem* it);

    //!  слот обработчика выбора "Средний спектр по области" из контекстного меню
    void onMenuAverageSpectr();

    //!  слот обработчика выбора "Дисперсия по области" из контекстного меню
    void onMenuStandardDeviation();

    //! расчет  дисперсии случайной величины Х
    double calcVariance(QVector<double> X);


private:
    Ui::PolygonManager *ui;
    HyperCube *m_cube;
    Attributes* m_attributes;
    int m_rows;
    int m_cols;
    bool m_polyCreationInProcess;
    bool m_flagDoubleClicked;
    QCustomPlot * m_cusPlot;
    QWidget * m_parent2D; // qwidget, а не Main2DWindow, т.к. его нельзя объявить
    QVector<Region> m_RegionArr;
    int m_currIndexRegion;// номер текущей (выделенной в списке) области
};

#endif // POLYGONMANAGER_H
