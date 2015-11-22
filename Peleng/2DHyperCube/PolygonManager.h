#ifndef POLYGONMANAGER_H
#define POLYGONMANAGER_H

#include <QMainWindow>

namespace Ui {
class PolygonManager;
}

class PolygonManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit PolygonManager(QWidget *parent = 0);
    ~PolygonManager();

private:
    Ui::PolygonManager *ui;
};

#endif // POLYGONMANAGER_H
