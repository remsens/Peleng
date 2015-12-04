

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QSharedPointer>
#include "../Library/QCustomPlot.h"
#include "../Library/HyperCube.h"
#include "../Library/Interfaces/ProcessingPluginInterface.h"
#include "ContrastWindow.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(HyperCube* ptrCube, Attributes* attr, QWidget *parent = 0);
    virtual ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

    void resizeAndRedraw(u::uint32 Ch1, u::uint32 Ch2, u::uint32 R1, u::uint32 R2, u::uint32 C1, u::uint32 C2);
    bool cantDelete();

public slots:

    void sliderCh1ValueChanged(int value);
    void sliderCh2ValueChanged(int value);
    void sliderX1ValueChanged(int value);
    void sliderX2ValueChanged(int value);
    void sliderY1ValueChanged(int value);
    void sliderY2ValueChanged(int value);
    void plotSpectr(uint x, uint y, uint z);
    void plotAlongLine(uint x1,uint x2,uint y1,uint y2,uint z1,uint z2);
    void addSpectr();
    void OnActionMedian1D_3Triggered();
    void OnActionMedian1D_5Triggered();
    void OnActionMedian1D_7Triggered();
    void OnActionMedian2D_3Triggered();
    void OnActionMedian2D_5Triggered();
    void OnActionMedian2D_7Triggered();

    void ActionNoiseSavitGolay2_3_5Toogled();
    void ActionNoiseSavitGolay2_3_7Toogled();
    void ActionNoiseSavitGolay2_3_9Toogled();
    void ActionNoiseSavitGolay4_5_7Toogled();
    void ActionNoiseSavitGolay4_5_9Toogled();

protected:
    void closeEvent(QCloseEvent *e);

private slots:

    void prepareToPlotSpectr();
    void startIsClicked();//нажато "Начало" из контекстного меню
    void finishIsClicked();
    void createLinePlotterSlot();
    void run2DCube();

    void contrast();
    void repaintWithContrast(int min, int max);
    void updateCube();
    void needToUpdate(bool needToUpdate);
    void ShowContextMenu(const QPoint& pos);
signals:
    void clicked();
    void sendXYZ(uint, uint, uint); //отправляет сигнал, по которому вызывается SpectrPlotter
    void signalPlotSpectr();
    void signalPlotAlongLine(uint,uint,uint,uint,uint,uint);
    void drawLabel(int, int, QString);
    void signalCurrentDataXYZ(uint,uint,uint);
    void flagsToolTip(QPoint, QString);

    void redrawSliders();
    void CanDelete();


protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
   // void contextMenuEvent(QContextMenuEvent* event)Q_DECL_OVERRIDE;
private:
    void makeObject();
    void loadData(HyperCube* ptrCube);
    void fillCubeSides();
    void create2RowChanSides(int ch1, int ch2, int r1, int r2);
    void create2ColChanSides(int ch1, int ch2, int c1, int c2);
    void createCubeSides();
    void scale_plus();
    void scale_minus();
    void makeTextures();
    void makeSidesImages(int chanNum);
    void SidesDestructor();
    void findMinMaxforColorMap(float thresholdLow = 0.02,float thresholdHigh = 0.99);
    void findAbsoluteMinMax();
    QImage from2Dmass2QImage(qint16 *data);
    QImage from2Dmass2QImage(qint16 **sidesData, int dim1, int dim2, int minContrast, int maxContrast, bool gray = false);
    void calcUintCords (float dataXf, float dataYf, float dataZf, u::uint16& dataXu,  u::uint16& dataYu, u::uint16& dataZu);
    void calcCenterCube(int Ch1, int Ch2, int R1, int R2, int C1, int C2);
    void evalDataCordsFromMouse(int mouseX, int mouseY);
    void Noise();
    void NoiseGolayAlgExecute();

private:
    Attributes* m_attributes;
    QColor clearColor;
    QPoint lastPos;
    QPoint globalPos;
    int ROWS ;//= 2449;
    int COLS ;//= 792;
    int CHNLS;// = 224;
    bool FLAGisInit = false;
    qint16 **data;
    qint16 ***sidesDataCH_RO = NULL;
    qint16 ***sidesDataCH_CO = NULL;
    qint16 ***sidesDataRO_CO = NULL;
    int xRot;
    int yRot;
    int zRot;
    QMatrix4x4 projection;//попытка использовать раздельные матрицы
    QMatrix4x4 m;
    QMatrix4x4 matrix;
    GLfloat ratio; //отношение высоты окна к ширине
    GLfloat nSca; // переменная отвечает за масштабирование обьекта
    GLfloat dx,dy; // для трансляции матрицы (сдвиг куба)
    GLfloat centerCubeX, centerCubeY, centerCubeZ;// центр текущего параллелепипеда
    QOpenGLTexture *textures[6];
    QImage *sidesImages[6];
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
    float kT = 1;
    float coords[6][4][3];
    int Ch1, Ch2, R1, R2, C1, C2; // хранят value слайдеров
    int prevChN, prevRowsN ;
    int minCMap,maxCMap;
    int minCMapSides,maxCMapSides;
    int absMin,absMax;
    HyperCube *m_pHyperCube;
    u::uint16 m_dataX, m_dataY, m_dataZ; // координаты (uint) ячейки массива data
    float m_dataXf, m_dataYf, m_dataZf; // // координаты (float) ячейки массива data
    uint m_x1, m_x2, m_y1, m_y2, m_z1, m_z2; //data координаты клика "Начало" и "Конец"

    bool firstWindowPlotter;

    QString strForLbl;
    QString strForLineHelp; //можно переделать и удалить это
    bool linePlotterIsActive = false;
    bool m_needToUpdate;
    bool cantDeleteVar;

    ContrastWindow *m_contrastTool;

};

#endif
