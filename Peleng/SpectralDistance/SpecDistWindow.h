#ifndef SPECDISTWINDOW_H
#define SPECDISTWINDOW_H

#include <QMainWindow>

namespace Ui {
class SpecDistWindow;
}

class SpecDistWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpecDistWindow(QWidget *parent = 0);
    ~SpecDistWindow();

signals:
    void rangeChanged(int);
    void calcMeth(int);

private slots:
    void on_sliderRange_valueChanged(int value);

    void on_buttonCalc_clicked();

private:
    Ui::SpecDistWindow *ui;
};

#endif // SPECDISTWINDOW_H
