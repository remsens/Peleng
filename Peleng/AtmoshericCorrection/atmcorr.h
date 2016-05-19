#ifndef ATMCORR_H
#define ATMCORR_H

#include <QDialog>

namespace Ui {
class AtmCorr;
}

class AtmCorr : public QDialog
{
    Q_OBJECT

public:
    explicit AtmCorr(QWidget *parent = 0);
    ~AtmCorr();

private:
    Ui::AtmCorr *ui;
    int makeCorrection();
};

#endif // ATMCORR_H
