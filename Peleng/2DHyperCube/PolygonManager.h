#ifndef POLYGONMANAGER_H
#define POLYGONMANAGER_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "Region.h"

namespace Ui {
class PolygonManager;
}

class PolygonManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit PolygonManager(int rows, int cols,
                            QCustomPlot *cusPlot, QWidget *parent2Dwindow);
    ~PolygonManager();
    //! функция,завершающая создание полигона
    void finishPolygonCreation();
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

    //! функция сохранения байтовой маски на диск
    void saveByteMask(QByteArray byteArr);

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

    void currentRowChanged(QModelIndex i1,QModelIndex i2); // возможно и не понадобится

    //! слот выбора цвета
    void pickColor();

    //! слот обработчика кнопки "Добавить регион интереса"
    void onButtonAddRegion();

    //! слот обработчика кнопки "Удалить регион интереса"
    void onButtonRemoveRegion();

    //!  слот обработчика кнопки "Добавить полигон"
    void onButtonAddPolygon();

private:
    Ui::PolygonManager *ui;
    int m_rows;
    int m_cols;
    bool m_polyCreationInProcess;
    bool m_flagDoubleClicked;
    QCustomPlot * m_cusPlot;
    QWidget * m_parent2D; // qwidget, а не Main2DWindow, т.к. его нельзя объявить
    QVector<QPolygon> m_polygonArr; //массив полигонов, принадлежащих одной области интереса. При создании новой области он будет очищаться
    QVector<Region> m_RegionArr;
};

#endif // POLYGONMANAGER_H
