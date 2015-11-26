#ifndef ZOOMIMAGE_H
#define ZOOMIMAGE_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>


enum Operation{NONE,ZOOM,MOVE};

class ZoomImage : public QLabel
{
    Q_OBJECT
public:
    explicit ZoomImage(QWidget *parent = 0);
    void setMainPixmap(QPixmap pixmap);





protected:

void paintEvent(QPaintEvent *event);
void mousePressEvent(QMouseEvent *event);
void mouseReleaseEvent(QMouseEvent *event);
void mouseMoveEvent(QMouseEvent *event);




signals:
    void updateSize();

public slots:

void ZoomInSetter();
void ZoomOutSetter();
void MoveSetter();

    \
private:
    void zoom();
    void zoomOut();
    void move();

    double scaledTo();


 QRect zoomRect;
 QRect currentRect;

 QPoint startPoint,startLeft;
 double scaled;
 QPixmap mainPixmap;
 QPixmap changePixmap;
 Operation currentOperation;




};

#endif // ZOOMIMAGE_H
