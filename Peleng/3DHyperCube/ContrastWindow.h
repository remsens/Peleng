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
    minMaxChanged(int,int);
private slots:
    void minChngdSlot(int val);
    void maxChngdSlot(int val);

private:
    Ui::ContrastWindow *ui;
    int MIN;
    int MAX;
};

#endif // CONTRASTWINDOW_H
