#ifndef PREVIEW2D_H
#define PREVIEW2D_H

#include <QWidget>

namespace Ui {
class Preview2D;
}

class Preview2D : public QWidget
{
    Q_OBJECT

public:
    explicit Preview2D(QWidget *parent = 0);
    virtual ~Preview2D();

    void Plot(double* data, const int rows, const int cols, const int numberOfActiveChannel = 0);

private:
    Ui::Preview2D *m_ui;
};

#endif // PREVIEW2D_H
