#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDockWidget>
#include <QGroupBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QSignalMapper>
#include <iostream>
#include <QGraphicsScene>
//TODO update qgscene on update

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiCanvasArea = new CanvasArea();
    qGScene = new QGraphicsScene(this);

    //qGScene->setSceneRect(uiCanvasArea->rect());
    qGScene->addWidget(uiCanvasArea);
    qGView = new QGraphicsView(this);
    qGView->setStyleSheet("background: transparent");
    //qGView->scale(2, 2);
    //qGView->setSceneRect(uiCanvasArea->rect());

    /*uiScrollArea = new QScrollArea(this);

    uiScrollArea->setBackgroundRole(QPalette::Dark);
    uiScrollArea->setWidget(uiCanvasArea);
    uiScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    uiScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    uiScrollArea->setWidgetResizable(false);
    uiScrollArea->setVisible(true);*/

    uiCanvasArea->setFixedWidth(uiCanvasArea->width());
    uiCanvasArea->setFixedHeight(uiCanvasArea->height());

    //setCentralWidget(uiScrollArea);
    createToolbar();
    qGView->setScene(qGScene);
    setCentralWidget(qGView);
    show();
}

void MainWindow::createToolbar() {
    QDockWidget *dock = new QDockWidget(tr("Tools"), this);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setFixedWidth(100);
    //toolbarButtons = new QListWidget(dock);

    signalMapper = new QSignalMapper(this);

    QGroupBox *groupBox = new QGroupBox(tr(""));
    groupBox->setMaximumHeight(140);


    QPushButton *buttonPencil = new QPushButton("P", this);
    buttonPencil->setCheckable(true);
    buttonPencil->setAutoExclusive(true);
    buttonPencil->setFixedWidth(25);
    buttonPencil->setFixedHeight(25);
    connect(buttonPencil, &QPushButton::released, [=]{
        uiCanvasArea->setMode(1);
    });


    QPushButton *buttonEraser = new QPushButton("E", this);
    buttonEraser->setCheckable(true);
    buttonEraser->setAutoExclusive(true);
    buttonEraser->setFixedWidth(25);
    buttonEraser->setFixedHeight(25);
    connect(buttonEraser, &QPushButton::released, [=]{
        uiCanvasArea->setMode(2);
    });


    QPushButton *buttonLine = new QPushButton("L", this);
    buttonLine->setCheckable(true);
    buttonLine->setAutoExclusive(true);
    buttonLine->setFixedWidth(25);
    buttonLine->setFixedHeight(25);
    connect(buttonLine, &QPushButton::released, [=]{
        uiCanvasArea->setMode(3);
    });

    QPushButton *buttonCurve = new QPushButton("C", this);
    buttonCurve->setCheckable(true);
    buttonCurve->setAutoExclusive(true);
    buttonCurve->setFixedWidth(25);
    buttonCurve->setFixedHeight(25);
    connect(buttonCurve, &QPushButton::released, [=]{
        uiCanvasArea->setMode(4);
    });

    QPushButton *buttonRectangle = new QPushButton("R", this);
    buttonRectangle->setCheckable(true);
    buttonRectangle->setAutoExclusive(true);
    buttonRectangle->setFixedWidth(25);
    buttonRectangle->setFixedHeight(25);
    connect(buttonRectangle, &QPushButton::released, [=]{
        uiCanvasArea->setMode(5);
    });


    QPushButton *buttonShape = new QPushButton("S", this);
    buttonShape->setCheckable(true);
    buttonShape->setAutoExclusive(true);
    buttonShape->setFixedWidth(25);
    buttonShape->setFixedHeight(25);
    connect(buttonShape, &QPushButton::released, [=]{
        uiCanvasArea->setMode(6);
    });


    QPushButton *buttonSelectArea = new QPushButton("A", this);
    buttonSelectArea->setCheckable(true);
    buttonSelectArea->setAutoExclusive(true);
    buttonSelectArea->setFixedWidth(25);
    buttonSelectArea->setFixedHeight(25);
    connect(buttonSelectArea, &QPushButton::released, [=]{
        uiCanvasArea->setMode(7);
    });


    QPushButton *buttonBucket = new QPushButton("B", this);
    buttonBucket->setCheckable(true);
    buttonBucket->setAutoExclusive(true);
    buttonBucket->setFixedWidth(25);
    buttonBucket->setFixedHeight(25);
    connect(buttonBucket, &QPushButton::released, [=]{
        uiCanvasArea->setMode(9);
    });

    QGridLayout *buttonLayout = new QGridLayout();
    buttonLayout->addWidget(buttonPencil, 0, 0);
    buttonLayout->addWidget(buttonEraser, 0, 1);
    buttonLayout->addWidget(buttonLine, 1, 0);
    buttonLayout->addWidget(buttonCurve, 1, 1);
    buttonLayout->addWidget(buttonRectangle, 2, 0);
    buttonLayout->addWidget(buttonShape, 2, 1);
    buttonLayout->addWidget(buttonSelectArea, 3, 0);
    buttonLayout->addWidget(buttonBucket, 3, 1);
    buttonLayout->setVerticalSpacing(10);
    buttonLayout->setHorizontalSpacing(10);
    groupBox->setLayout(buttonLayout);
    /*QButtonGroup *buttonGroup = new QButtonGroup();
    buttonGroup->addButton(buttonPencil);
    buttonGroup->addButton(buttonEraser);
    buttonGroup->addButton(buttonLine);
    buttonGroup->addButton(buttonCurve);
    buttonGroup->addButton(buttonRectangle);
    buttonGroup->addButton(buttonShape);
    buttonGroup->addButton(buttonSelectArea);
    buttonGroup->addButton(buttonBucket);*/
    //groupBox->setLayout(buttonGroup);

    dock->setWidget(groupBox);
    //toolbarButtons->addItems()
    addDockWidget(Qt::RightDockWidgetArea, dock);


    QDockWidget *colorDock = new QDockWidget(tr("Color"), this);
    colorDock->setFeatures(dock->features() & ~QDockWidget::DockWidgetClosable);
    colorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    colorDock->setFixedWidth(100);


    QGroupBox *groupColorBox = new QGroupBox(tr(""));
    groupColorBox->setMaximumHeight(150);

    buttonPrimaryColor = new QPushButton("", this);
    buttonPrimaryColor->setFixedWidth(25);
    buttonPrimaryColor->setFixedHeight(25);
    connect(buttonPrimaryColor, &QPushButton::released, [=]{
        uiCanvasArea->setColor(0);
        QColor col = uiCanvasArea->getCanvasPrimaryColor();
        if(col.isValid()) {
           QString qss = QString("background-color: %1").arg(col.name());
           buttonPrimaryColor->setStyleSheet(qss);
        }
    });
    QColor col = uiCanvasArea->getCanvasPrimaryColor();
    if(col.isValid()) {
       QString qss = QString("background-color: %1").arg(col.name());
       buttonPrimaryColor->setStyleSheet(qss);
    }

    buttonSecondaryColor = new QPushButton("", this);
    buttonSecondaryColor->setFixedWidth(25);
    buttonSecondaryColor->setFixedHeight(25);
    connect(buttonSecondaryColor, &QPushButton::released, [=]{
        uiCanvasArea->setColor(1);
        QColor col = uiCanvasArea->getCanvasSecondaryColor();
        if(col.isValid()) {
           QString qss = QString("background-color: %1").arg(col.name());
           buttonSecondaryColor->setStyleSheet(qss);
        }
    });
    col = uiCanvasArea->getCanvasSecondaryColor();
    if(col.isValid()) {
       QString qss = QString("background-color: %1").arg(col.name());
       buttonSecondaryColor->setStyleSheet(qss);
    }
    QGridLayout *buttonColorLayout = new QGridLayout();
    buttonColorLayout->addWidget(buttonPrimaryColor, 0, 0);
    buttonColorLayout->addWidget(buttonSecondaryColor, 0, 1);


    brushSizeSlider = new QSlider(Qt::Vertical);
    brushSizeSlider->setFocusPolicy(Qt::StrongFocus);
    brushSizeSlider->setTickPosition(QSlider::TicksBothSides);
    brushSizeSlider->setTickInterval(10);
    brushSizeSlider->setSingleStep(1);
    connect(brushSizeSlider, &QSlider::valueChanged, [=]{uiCanvasArea->setBrushSize(brushSizeSlider->value());});
    buttonColorLayout->addWidget(brushSizeSlider, 1, 0);

    QPushButton *buttonFill = new QPushButton("F", this);
    buttonFill->setCheckable(true);
    buttonFill->setFixedWidth(25);
    buttonFill->setFixedHeight(25);
    connect(buttonBucket, &QPushButton::released, [=]{
        uiCanvasArea->changeFill();
    });
    buttonColorLayout->addWidget(buttonFill, 1, 1);

    groupColorBox->setLayout(buttonColorLayout);

    colorDock->setWidget(groupColorBox);
    addDockWidget(Qt::RightDockWidgetArea, colorDock);



}

MainWindow::~MainWindow()
{
    delete ui;
}

