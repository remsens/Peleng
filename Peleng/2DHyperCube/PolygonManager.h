#ifndef POLYGONMANAGER_H
#define POLYGONMANAGER_H

#include <QMainWindow>
#include "Main2DWindow.h"
#include "../Library/QCustomPlot.h"


namespace Ui {
class PolygonManager;
}

class PolygonManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit PolygonManager(int rows, int cols,
                            QCustomPlot *cusPlot, Main2DWindow *parent2Dwindow);
    ~PolygonManager();
    void finishPolygonCreation();
public slots:
    void createPolygonSlot();
    void addPolygonPoint(uint x, uint y);
private:
     void drawLine(QPoint p1, QPoint p2, QColor color);
private:
    Ui::PolygonManager *ui;
    int m_rows;
    int m_cols;
    bool m_polyCreationInProcess;
    bool m_flagDoubleClicked;
    QCustomPlot * m_cusPlot;
    Main2DWindow *m_parent2D;
    QVector<QPolygon> polygonArr;
};

#endif // POLYGONMANAGER_H
