#ifndef RGBIMAGEFORM_H
#define RGBIMAGEFORM_H

#include <QWidget>
#include "../Library/HyperCube.h"
#include "../Library/Attributes/Attributes.h"

namespace Ui {
class RgbImageForm;
}

class RgbImageForm : public QWidget
{
    Q_OBJECT

public:
    explicit RgbImageForm(QWidget *parent = 0);
    ~RgbImageForm();

    void plotImage(HyperCube *cube, Attributes *attr);

private:
    Ui::RgbImageForm *ui;
};

#endif // RGBIMAGEFORM_H
