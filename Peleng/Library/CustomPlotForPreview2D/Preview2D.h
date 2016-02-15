#ifndef PREVIEW2D_H
#define PREVIEW2D_H

#include <QWidget>
#include "../QCustomPlot.h"
#include "../../Library/HyperCube.h"
#include "../../Library/Attributes/Attributes.h"
namespace Ui {
class Preview2D;
}

class Preview2D : public QWidget
{
    Q_OBJECT

public:
    explicit Preview2D(QWidget *parent = 0);
    virtual ~Preview2D();

    void Plot(double* data, const int rows, const int cols, const QString& title = QString()
            ,HyperCube* cube = nullptr, Attributes* attr = nullptr );
    void plotPointsOn2D(QVector<double> x, QVector<double> y);

private slots:
    void ShowContextMenu(QPoint pos);
    void prepareToPlotSpectr();
    void mousePressOnColorMap(QMouseEvent* e);
    void on_SliderMin_valueChanged(int value);
    void on_SliderMax_valueChanged(int value);
private:
    Ui::Preview2D *m_ui;
    QCustomPlot* m_cPlot;
    QCPColorMap* colorMap;
    int m_dataX, m_dataY;
    int m_rows, m_cols;
    HyperCube* m_cube;
    Attributes* m_attr;
};

#endif // PREVIEW2D_H
