#ifndef LINEPLOTTERWINDOW_H
#define LINEPLOTTERWINDOW_H

#include <QMainWindow>
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

class LinePlotterWindow : public QMainWindow
{
    Q_OBJECT

public:
    // передаем на случай, если нам необходимы доступные плагины
    // и если нам необходимо изменить поля аттрибутов
    explicit LinePlotterWindow(Attributes* attr, QWidget *parent = 0);
    virtual ~LinePlotterWindow();
    void plotSpectrLine(HyperCube* pCube, uint x1, uint x2, uint y1, uint y2, uint z1, uint z2);

private:
    void closeEvent(QCloseEvent *);

signals:
    void closeLinePlotterWindow(LinePlotterWindow*);

private:
    void setupUI();

private:
    QWidget *centralWidget;
    QCustomPlot* m_customPlot;
    QVBoxLayout *verticalLayout;
    Attributes* m_attributes;
};

#endif // LINEPLOTTERWINDOW_H

