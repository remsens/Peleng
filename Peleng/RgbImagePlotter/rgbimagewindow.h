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
    explicit RgbImageWindow(QWidget *parent = 0);
    ~RgbImageWindow();

public slots:

    void plotImage(HyperCube *cube, Attributes *attr, SettingsDialog *settings);



//zoom
protected:
    bool eventFilter(QObject *obj, QEvent *event);
  //  void paintEvent(QPaintEvent *event);

private slots:
    void updateSize();
    void ZoomIn();
    void ZoomOut();

private:
    QRect zoomRect;
    bool drawing ;
    Ui::RgbImageWindow *ui;
    QPixmap mainPixmap;
    QPainter painter;
};

#endif // RGBIMAGEWINDOW_H
