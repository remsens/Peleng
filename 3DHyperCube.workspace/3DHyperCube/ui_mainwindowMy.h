/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWMY_H
#define UI_MAINWINDOWMY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"
#include "HyperCube.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout;
    GLWidget *widgetHyperCube;
    QGridLayout *gridLayout_2;
    QScrollBar *horizontalScrollBar_Y2;
    QScrollBar *horizontalScrollBar_Ch2;
    QScrollBar *horizontalScrollBar_Ch1;
    QScrollBar *horizontalScrollBar_X2;
    QScrollBar *horizontalScrollBar_Y1;
    QScrollBar *horizontalScrollBar_X1;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(HyperCube *ptrCube,QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1073, 797);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_5 = new QVBoxLayout(centralwidget);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widgetHyperCube = new GLWidget(ptrCube,centralwidget);
        widgetHyperCube->setObjectName(QStringLiteral("widgetHyperCube"));

        verticalLayout->addWidget(widgetHyperCube);


        verticalLayout_5->addLayout(verticalLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout_2->setHorizontalSpacing(6);
        gridLayout_2->setContentsMargins(0, -1, -1, -1);
        horizontalScrollBar_Y2 = new QScrollBar(centralwidget);
        horizontalScrollBar_Y2->setObjectName(QStringLiteral("horizontalScrollBar_Y2"));
        horizontalScrollBar_Y2->setLayoutDirection(Qt::LeftToRight);
        horizontalScrollBar_Y2->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalScrollBar_Y2, 2, 5, 1, 1);

        horizontalScrollBar_Ch2 = new QScrollBar(centralwidget);
        horizontalScrollBar_Ch2->setObjectName(QStringLiteral("horizontalScrollBar_Ch2"));
        horizontalScrollBar_Ch2->setLayoutDirection(Qt::LeftToRight);
        horizontalScrollBar_Ch2->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalScrollBar_Ch2, 0, 5, 1, 1);

        horizontalScrollBar_Ch1 = new QScrollBar(centralwidget);
        horizontalScrollBar_Ch1->setObjectName(QStringLiteral("horizontalScrollBar_Ch1"));
        horizontalScrollBar_Ch1->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalScrollBar_Ch1, 0, 4, 1, 1);

        horizontalScrollBar_X2 = new QScrollBar(centralwidget);
        horizontalScrollBar_X2->setObjectName(QStringLiteral("horizontalScrollBar_X2"));
        horizontalScrollBar_X2->setLayoutDirection(Qt::LeftToRight);
        horizontalScrollBar_X2->setValue(0);
        horizontalScrollBar_X2->setOrientation(Qt::Horizontal);
        horizontalScrollBar_X2->setInvertedControls(true);

        gridLayout_2->addWidget(horizontalScrollBar_X2, 1, 5, 1, 1);

        horizontalScrollBar_Y1 = new QScrollBar(centralwidget);
        horizontalScrollBar_Y1->setObjectName(QStringLiteral("horizontalScrollBar_Y1"));
        horizontalScrollBar_Y1->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalScrollBar_Y1, 2, 4, 1, 1);

        horizontalScrollBar_X1 = new QScrollBar(centralwidget);
        horizontalScrollBar_X1->setObjectName(QStringLiteral("horizontalScrollBar_X1"));
        horizontalScrollBar_X1->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalScrollBar_X1, 1, 4, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(26, 0));
        label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label, 0, 3, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 0, 2, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 1, 2, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setMinimumSize(QSize(26, 0));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_6, 1, 3, 1, 1);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 2, 0, 1, 1);

        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_2->addWidget(label_8, 2, 2, 1, 1);

        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy1.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy1);
        label_9->setMinimumSize(QSize(26, 0));
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_9, 2, 3, 1, 1);

        label_19 = new QLabel(centralwidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        sizePolicy1.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy1);
        label_19->setMinimumSize(QSize(26, 0));
        label_19->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_19, 0, 1, 1, 1);

        label_20 = new QLabel(centralwidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        sizePolicy1.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy1);
        label_20->setMinimumSize(QSize(26, 0));
        label_20->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_20, 1, 1, 1, 1);

        label_21 = new QLabel(centralwidget);
        label_21->setObjectName(QStringLiteral("label_21"));
        sizePolicy1.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy1);
        label_21->setMinimumSize(QSize(26, 0));
        label_21->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_21, 2, 1, 1, 1, Qt::AlignHCenter);

        gridLayout_2->setColumnStretch(0, 1);
        gridLayout_2->setColumnStretch(1, 1);
        gridLayout_2->setColumnStretch(2, 1);
        gridLayout_2->setColumnStretch(3, 1);
        gridLayout_2->setColumnStretch(4, 100);
        gridLayout_2->setColumnStretch(5, 100);

        verticalLayout_5->addLayout(gridLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1073, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(horizontalScrollBar_Ch1, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderCh1ValueChanged(int)));
        QObject::connect(horizontalScrollBar_Ch2, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderCh2ValueChanged(int)));
        QObject::connect(horizontalScrollBar_X1, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderX1ValueChanged(int)));
        QObject::connect(horizontalScrollBar_X2, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderX2ValueChanged(int)));
        QObject::connect(horizontalScrollBar_Y1, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderY1ValueChanged(int)));
        QObject::connect(horizontalScrollBar_Y2, SIGNAL(valueChanged(int)), widgetHyperCube, SLOT(sliderY2ValueChanged(int)));
        QObject::connect(horizontalScrollBar_Ch1, SIGNAL(valueChanged(int)), label_19, SLOT(setNum(int)));
        QObject::connect(horizontalScrollBar_Ch2, SIGNAL(valueChanged(int)), label, SLOT(setNum(int)));
        QObject::connect(horizontalScrollBar_X1, SIGNAL(valueChanged(int)), label_20, SLOT(setNum(int)));
        QObject::connect(horizontalScrollBar_X2, SIGNAL(valueChanged(int)), label_6, SLOT(setNum(int)));
        QObject::connect(horizontalScrollBar_Y1, SIGNAL(valueChanged(int)), label_21, SLOT(setNum(int)));
        QObject::connect(horizontalScrollBar_Y2, SIGNAL(valueChanged(int)), label_9, SLOT(setNum(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\223\320\270\320\277\320\265\321\200\320\272\321\203\320\261", 0));
        label->setText(QApplication::translate("MainWindow", "0", 0));
        label_2->setText(QApplication::translate("MainWindow", ":", 0));
        label_3->setText(QApplication::translate("MainWindow", "\320\232\320\260\320\275\320\260\320\273", 0));
        label_4->setText(QApplication::translate("MainWindow", "\320\241\321\202\321\200\320\276\320\272\320\270", 0));
        label_5->setText(QApplication::translate("MainWindow", ":", 0));
        label_6->setText(QApplication::translate("MainWindow", "0", 0));
        label_7->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\276\320\273\320\261\321\206\321\213", 0));
        label_8->setText(QApplication::translate("MainWindow", ":", 0));
        label_9->setText(QApplication::translate("MainWindow", "0", 0));
        label_19->setText(QApplication::translate("MainWindow", "0", 0));
        label_20->setText(QApplication::translate("MainWindow", "0", 0));
        label_21->setText(QApplication::translate("MainWindow", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWMY_H
