#ifndef PLOTTERWINDOW_H
#define PLOTTERWINDOW_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"
#include "../Library/Spectr.h"
#include "../Library/structures.h"
#include "SpectrumComparatorFromExternalLibraries.h"

namespace Ui {
class PlotterWindow;
}

class PlotterWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit PlotterWindow(HyperCube* cube, Attributes* attr, QWidget *parent = 0);
    virtual ~PlotterWindow();

    void plotSpectr(QVector<double>& dataX, QVector<double>& dataY, QString& graphName = QString(""));
    bool getIsHold(){return m_hold;}
    void setMeasurement(Measurements& measurement) {m_measurement = measurement;}
    Measurements getGraphMeasurement() {return m_measurement;}
protected:
    void resizeEvent(QResizeEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


signals:
    void closePlotterWindow(PlotterWindow* w);
private:
    void closeEvent(QCloseEvent *);
    void NoiseMedianAlgExecute();
    void NoiseGolayAlgExecute();
private slots:
    void graphClicked(QCPAbstractPlottable *plottable);
    void contextMenuRequest(QPoint pos);
    void mouseMoveRequest(QMouseEvent* e);
    void removeSelectedGraph();
    void removeAllExceptSelectedGraph();
    //! слот по клику из опций "Отображать значения под курсором"
    void onActionValues(bool flag);
    //! слот по клику из опций "Отображать точки графика"
    void onActionPoints(bool flag);
    //! слот по клику из опций "привестик длинам волн гиперкуба"
    void onActionInterplol();

    void on_horizontalSlider_end_valueChanged(int value);
    void on_horizontalSlider_start_valueChanged(int value);

    void on_horizontalSlider_start_sliderMoved(int position);

    void on_horizontalSlider_end_sliderMoved(int position);
    void findInSpectralLib();
public slots:
    void on_actionHold_toggled(bool value);
    void on_actionSave_toggled();
    void ActionNoise3MedianToggled();
    void ActionNoise5MedianToggled();
    void ActionNoise7MedianToggled();


    void ActionNoiseSavitGolay2_5Toogled();
    void ActionNoiseSavitGolay2_7Toogled();
    void ActionNoiseSavitGolay2_9Toogled();
    void ActionNoiseSavitGolay3_5Toogled();
    void ActionNoiseSavitGolay3_7Toogled();
    void ActionNoiseSavitGolay3_9Toogled();
    void ActionNoiseSavitGolay4_7Toogled();
    void ActionNoiseSavitGolay4_9Toogled();
    void ActionNoiseSavitGolay5_7Toogled();
    void ActionNoiseSavitGolay5_9Toogled();

    void ActionEvclideDistToogled();
    void ActionSpectraAngleToogled();
    void ActionSpectraDistanceToogled();
    void ActionSpectraCorrelationToogled();
    void ActionSIDToogled();
    void ActionEntropyToogled();

    void ActionNormalization();
    void AddSpectr();
    void CompareSpectrWithCube();
    void PrepareToCompare();
private:
    bool m_hold;
    bool m_valuesFlag;
    bool m_pointsFlag;
    Ui::PlotterWindow *ui;
    QCustomPlot *m_customPlot;
    QCPItemText *textValues;
    QCPItemStraightLine *vertLine;
    QCPItemStraightLine *horizLine;
    double minY;
    double maxY;
    QSize initSize;
    int autoTickCountX;
    int autoTickCountY;
    HyperCube* m_cube;
    Attributes* m_attributes;
    QList<DescriptionSpectr> m_descriptionExternalSpectr;
    QVector<double> m_xArr;
    QVector<double> m_yArr;
    QList<Spectr*> m_listSpectr;
    Spectr* m_selectedSpectr;
    Measurements m_measurement;
    QCPItemStraightLine *leftLine, *rightLine;
    SpectrumComparatorFromExternalLibraries::CompareMode m_modeCompare;
};

#endif // PLOTTERWINDOW_H
