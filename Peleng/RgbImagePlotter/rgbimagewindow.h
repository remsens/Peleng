#ifndef RGBIMAGEWINDOW_H
#define RGBIMAGEWINDOW_H

#include <QMainWindow>
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"
#include "settingsdialog.h"
#include "../Library/QCustomPlot.h"

#include "zoomimage.h"
namespace Ui {
class RgbImageWindow;
}

class RgbImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RgbImageWindow(HyperCube *cube, QWidget *parent = 0);
    ~RgbImageWindow();

public slots:

    void plotImage(Attributes *attr, SettingsDialog *settings);

private:
    void showAtCustomplot(QImage image);
    void initCustomplotSettings();
    void setAspectRatio();
    void scaleRulesForAxis(QCPAxis *axis, double rangeUpper, QCPRange newRange, QCPRange oldRange);

private slots:
    void updateSize();
    void xRangeChanged(QCPRange newRange, QCPRange oldRange);
    void yRangeChanged(QCPRange newRange, QCPRange oldRange);

private:
    Ui::RgbImageWindow *ui;
    HyperCube *m_cube;
};

#endif // RGBIMAGEWINDOW_H
