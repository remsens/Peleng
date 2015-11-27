#ifndef PLOTTERWINDOW_H
#define PLOTTERWINDOW_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

namespace Ui {
class PlotterWindow;
}

class PlotterWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit PlotterWindow(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    virtual ~PlotterWindow();

    void plotSpectr(uint dataX = 0, uint dataY = 0 );
    bool getIsHold(){return m_hold;}

    void chooseStep();
protected:
    void resizeEvent(QResizeEvent * event);

signals:
    void closePlotterWindow(PlotterWindow* w);
private:
    void closeEvent(QCloseEvent *);
    void NoiseAlgExecute();
private slots:
    void graphClicked(QCPAbstractPlottable *plottable);
public slots:
    void on_actionHold_toggled(bool value);
    void on_actionSave_toggled();
    void ActionNoise3Toggled();
    void ActionNoise5Toggled();
    void ActionNoise7Toggled();

private:
    bool m_hold;
    Ui::PlotterWindow *ui;
    QCustomPlot *m_customPlot;
    double minY;
    double maxY;
    QSize initSize;
    int autoTickCountX;
    int autoTickCountY;
    HyperCube* m_cube;
    Attributes* m_attributes;
    QAction* m_actionSave;

    QAction* m_actionNoise3;
    QAction* m_actionNoise5;
    QAction* m_actionNoise7;
    QMenu*   m_menuNoise;
    QMenu*   m_menuMedianNoise;
    QList<Attributes::DescriptionSpectr> m_descriptionExternalSpectr;
    QVector<double> m_xArr;
    QVector<double> m_yArr;

};

#endif // PLOTTERWINDOW_H
