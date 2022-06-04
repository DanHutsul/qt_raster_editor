#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiCanvasArea = new CanvasArea();

    uiScrollArea = new QScrollArea(this);

    uiScrollArea->setBackgroundRole(QPalette::Dark);
    uiScrollArea->setWidget(uiCanvasArea);
    uiScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    uiScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    uiScrollArea->setWidgetResizable(false);
    uiScrollArea->setVisible(true);

    setCentralWidget(uiScrollArea);
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

