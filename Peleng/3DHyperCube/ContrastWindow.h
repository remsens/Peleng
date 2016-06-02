#ifndef CONTRASTWINDOW_H
#define CONTRASTWINDOW_H

#include <QWidget>

namespace Ui {
class ContrastWindow;
}

class ContrastWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ContrastWindow(int absMin, int absMax, int initMin, int initMax,  QWidget *parent = 0);
    ~ContrastWindow();
    void setMinMax(int absMin, int absMax,int initMin, int initMax);
signals:
    void minMaxChanged(int,int);
private slots:
    void minChngdSlot(int val);
    void maxChngdSlot(int val);
//    void showMinLabel(int val);
//    void showMaxLabel(int val);
private:
    int intFromDouble(double val);
    double doubleFromInt(int val);
private:
    Ui::ContrastWindow *ui;
    int MIN;
    int MAX;
    int dpi = 100;
};

#endif // CONTRASTWINDOW_H
