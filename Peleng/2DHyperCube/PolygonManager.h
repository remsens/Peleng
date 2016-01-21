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
public slots:
    //! слот соединяющийся с сигналом по экшену в контестном меню с 2D куба
    void createPolygonSlot();

    //! слот обрабатывающий двойной клик по колормэпу 2D куба
    void mouseDblClickOnParentColorMap( QMouseEvent *);
private:
    //! функция отрисовки линии на customPlot
    void drawLine(QPoint p1, QPoint p2, QColor color);

    //! функция создания байтовой маски из полигонов
    QByteArray byteMaskFromPolygons(QVector<QPolygon> polygonArr);

    //! функция создания байтовой маски из 2 других байтовых масок
    QByteArray byteMaskFrom2ByteMasks(QByteArray arr1, QByteArray arr2);

    //! функция сохранения байтовой маски на диск
    void saveByteMask(QByteArray byteArr, QString fileName);

    //! функция загрузки байтовой маски с диска
    QByteArray loadByteMaskFromFile();

    //! функция создания изображения(с альфа-каналом) из байтовой маски
    QImage imageFromByteMask(QByteArray byteArr,QColor color);

    //! функция отрисовки изображения на m_cusPlot
    void drawImage(QImage mask);

private slots:


    //! слот добавления точки полигона по клику
    void addPolygonPoint(uint x, uint y);

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

    //! расчет  СКО случайной величины Х
    double calcStandardDeviation(QVector<double> X);


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
    QVector<QPolygon> m_polygonArr; //массив полигонов, принадлежащих одной области интереса. При создании новой области он будет очищаться
    QVector<Region> m_RegionArr;
    int m_currIndexRegion;// номер текущей (выделенной в списке) области
};

#endif // POLYGONMANAGER_H
