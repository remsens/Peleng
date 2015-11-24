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
    explicit PolygonManager( int rows, int cols,
                            QCustomPlot *cusPlot, QWidget *parent = 0);
    ~PolygonManager();

private:
    Ui::PolygonManager *ui;
    int m_rows;
    int m_cols;
    QCustomPlot * m_cusPlot;
};

#endif // POLYGONMANAGER_H
