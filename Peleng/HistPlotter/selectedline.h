#ifndef SELECTEDLINE_H
#define SELECTEDLINE_H

#include <QObject>
#include "../Library/QCustomPlot.h"

class SelectedLine : public QCPItemLine
{
public:
    SelectedLine(QCustomPlot *parentPlot);

signals:

protected:
    void selectEvent(QMouseEvent *event, bool additive, const QVariant & details, bool *selectionStateChanged);
  /*  void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);*/

public slots:
    void testSlot(bool test);
private:
    bool isSelected;
};

#endif // SELECTEDLINE_H
