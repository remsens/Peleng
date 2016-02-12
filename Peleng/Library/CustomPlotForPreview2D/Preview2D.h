#ifndef PREVIEW2D_H
#define PREVIEW2D_H

#include <QWidget>
#include "../QCustomPlot.h"
namespace Ui {
class Preview2D;
}

class Preview2D : public QWidget
{
    Q_OBJECT

public:
    explicit Preview2D(QWidget *parent = 0);
    virtual ~Preview2D();

    void Plot(double* data, const int rows, const int cols, const QString& title = QString());
    void plotPointsOn2D(QVector<double> x, QVector<double> y);

private slots:
    void ShowContextMenu(const QPoint pos);
    void prepareToPlotSpectr();
    void mousePressOnColorMap(QMouseEvent* e);
    void on_SliderMin_valueChanged(int value);
    void on_SliderMax_valueChanged(int value);
private:
    Ui::Preview2D *m_ui;
    QCustomPlot* m_cPlot;
    QCPColorMap* colorMap;
    int m_dataX, m_dataY;
};

#endif // PREVIEW2D_H
