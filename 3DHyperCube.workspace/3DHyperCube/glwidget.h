/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "qcustomplot.h"
#include "HyperCube.h"
#include "GetHyperCube.h"
#include "PlotterWindow.h"
#include "PlotterAlongLine.h"
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(HyperCube* ptrCube, QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

public slots:

    void sliderCh1ValueChanged(int value);
    void sliderCh2ValueChanged(int value);
    void sliderX1ValueChanged(int value);
    void sliderX2ValueChanged(int value);
    void sliderY1ValueChanged(int value);
    void sliderY2ValueChanged(int value);
    void BrightCheckClicked();
    void plotSpectr(uint x, uint y, uint z);
    void plotAlongLine(uint x1,uint x2,uint y1,uint y2,uint z1,uint z2);
    void deleteSpectrWindows();

private slots:

    void prepareToPlotSpectr();
    void startIsClicked();//нажато "Начало" из контекстного меню
    void finishIsClicked();

signals:
    void sendXYZ(uint, uint, uint);
    void signalPlotSpectr();
    void signalPlotAlongLine(uint,uint,uint,uint,uint,uint);
    void drawLabel(int, int, QString);
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent* event)Q_DECL_OVERRIDE;
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
    QImage from2Dmass2QImage(qint16 *data);
    QImage from2Dmass2QImage(qint16 **sidesData,int dim1,int dim2,bool gray = false);
    void createMenus();
    void calcUintCords (float dataXf, float dataYf, float dataZf, u::uint16& dataXu,  u::uint16& dataYu, u::uint16& dataZu);
    void calcCenterCube(int Ch1, int Ch2, int R1, int R2, int C1, int C2);
    void evalDataCordsFromMouse(int mouseX, int mouseY);

    QMenu* pContextMenu;
    QAction* pPlotAction;
    QAction* pDeletePlotsAction;
    QAction* pSetStartAction;
    QAction* pSetFinishAction;
    QColor clearColor;
    QPoint lastPos;
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
    float coords[6][4][3] = {
                             { { +kT, -1, -1 }, { -kT, -1, -1 }, { -kT, +1, -1 }, { +kT, +1, -1 } },
                             { { +kT, +1, -1 }, { -kT, +1, -1 }, { -kT, +1, +1 }, { +kT, +1, +1 } },
                             { { +kT, -1, +1 }, { +kT, -1, -1 }, { +kT, +1, -1 }, { +kT, +1, +1 } },
                             { { -kT, -1, -1 }, { -kT, -1, +1 }, { -kT, +1, +1 }, { -kT, +1, -1 } },
                             { { +kT, -1, +1 }, { -kT, -1, +1 }, { -kT, -1, -1 }, { +kT, -1, -1 } },
                             { { -kT, -1, +1 }, { +kT, -1, +1 }, { +kT, +1, +1 }, { -kT, +1, +1 } }
                            };
    int Ch1, Ch2, R1, R2, C1, C2; // хранят value слайдеров
    int prevChN, prevRowsN ;
    int minCMap,maxCMap;
    GetHyperCube* m_cube;
    HyperCube *m_pHyperCube;
    u::uint16 m_dataX, m_dataY, m_dataZ; // координаты (uint) ячейки массива data
    float m_dataXf, m_dataYf, m_dataZf; // // координаты (float) ячейки массива data
    uint m_x1, m_x2, m_y1, m_y2, m_z1, m_z2; //data координаты клика "Начало" и "Конец"
    PlotterWindow* windowPlotter = 0;
    PlotterAlongLine *pWidgLine = 0;
    QVector<PlotterWindow*> windowsArr; //для хранения указателей на плоттер окна и их удаления
    //QVector<PlotterAlongLine*> windowsArrLines; //для хранения указателей на плоттер(вдоль линии) окна и их удаления
    QString strForLbl;
};

#endif
