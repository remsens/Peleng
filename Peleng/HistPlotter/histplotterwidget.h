#ifndef HISTPLOTTERWIDGET_H
#define HISTPLOTTERWIDGET_H

#include <QDialog>

#include <QProgressDialog>


#include "../Library/Utils/typeconvertor.h"
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

namespace Ui {
class HistPlotterWidget;
}

class HistPlotterWidget : public QDialog
{
    Q_OBJECT

public:
    explicit HistPlotterWidget(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    ~HistPlotterWidget();


protected:
    void accept();


signals:

    void Close(HistPlotterWidget* obj);

private slots:
    void on_leftSlider_sliderMoved(int position);
    void on_rightSlider_sliderMoved(int position);
    void on_leftSlider_valueChanged(int value);
    void on_rightSlider_valueChanged(int value);

    void PlotDefault();
    void sumBoundary();
    void Gauss();


    void on_gaussChBox_clicked(bool checked);
    void on_sumBoundaryChBox_clicked(bool checked);

private:


    Ui::HistPlotterWidget *ui;
    QCPBars *BarsBefore, *BarsAfter;


    double *data;

    quint32 Channel;
    HyperCube *m_cube;
    Attributes* m_attributes;
    QCPItemStraightLine *leftLine, *rigthLine;
};

#endif // HISTPLOTTERWIDGET_H
