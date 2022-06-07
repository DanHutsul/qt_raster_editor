#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSignalMapper>
#include <QListWidget>
#include <QMainWindow>
#include <QScrollArea>
#include <canvasarea.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QInputDialog>
#include <QSpinBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void createToolbar();
    void scaleCanvas(qreal value);
private:
    Ui::MainWindow *ui;
    //QScrollArea *uiScrollArea;
    CanvasArea *uiCanvasArea;
    QListWidget *toolbarButtons;
    //QInputDialog *brushSizeInput;
    QSpinBox *brushSizeInput;
    QSignalMapper *signalMapper;
    QPushButton *buttonPrimaryColor;
    QPushButton *buttonSecondaryColor;
    //QSlider *brushSizeSlider;
    QGraphicsScene *qGScene;
    QGraphicsView *qGView;
    qreal canvasScale = 1;
};
#endif // MAINWINDOW_H
