#include "PlotterWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlotterWindow w;

    GetHyperCube* getCube = new GetHyperCube();// открывает данные авириса с диска и загружает их в память

    w.show();
    w.plotSpectr(getCube->GetCube(),400,300);
    w.plotSpectr(getCube->GetCube(),800,200);

    return a.exec();
}
