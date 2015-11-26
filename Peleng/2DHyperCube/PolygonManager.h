#ifndef POLYGONMANAGER_H
#define POLYGONMANAGER_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"


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

public slots:

private:
    //! функция отрисовки линии на customPlot
    void drawLine(QPoint p1, QPoint p2, QColor color);

    //! функция,завершающая создание полигона
    void finishPolygonCreation();

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
    //! слот загрузки байтовой маски с диска
    void createPolygonSlot();

    //! слот добавления точки полигона по клику
    void addPolygonPoint(uint x, uint y);

    //! слот обработки контекстного tableWidgetа
    void tableContextMenuRequest(QPoint pos);

    void currentRowChanged(QModelIndex i1,QModelIndex i2);
private:
    Ui::PolygonManager *ui;
    int m_rows;
    int m_cols;
    bool m_polyCreationInProcess;
    bool m_flagDoubleClicked;
    QCustomPlot * m_cusPlot;
    QWidget * m_parent2D; // qwidget, а не Main2DWindow, т.к. его нельзя объявить
    QVector<QPolygon> polygonArr;
};

#endif // POLYGONMANAGER_H
