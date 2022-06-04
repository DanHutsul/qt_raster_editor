#ifndef CANVASAREA_H
#define CANVASAREA_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainterPath>
class CanvasArea : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasArea(QWidget *parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void canvasDrawPencil(QPainter *painter);
    void canvasDrawEraser(QPainter *painter);
    void canvasDrawLine(QPainter *painter);
    void canvasDrawCurve(QPainter *painter);
    void canvasDrawRectangle(QPainter *painter);
    void canvasDrawCustomShape(QPainter *painter);



    //void canvasDrawPolygon(QPainter *painter, int n);
    void canvasDrawCircle(bool updateFlag = false);
private:
    //QPainter canvasPainter;
    QImage *canvasImage;
    QImage *canvasImageBackup;
    //QString *fileName;
    int canvasMode;
    //std::array<std::function<void()>, 1> canvasDrawFunctions = {&canvasDrawPencil};
    QColor canvasPrimaryColor;
    QColor canvasSecondaryColor;
    bool canvasPrimaryColorChosen = true;

    QPolygon canvasCustomShapePolygon;
    bool canvasCustomShapePolygonLooped = false;

    QPainterPath *canvasCurvePath;
    int canvasCurvePathPoints = 0;
    QPoint canvasCurvePathStart;
    QPoint canvasCurvePathEnd;
    QPoint canvasCurvePathMiddle1;
    QPoint canvasCurvePathMiddle2;

    bool canvasMouseReleased = false;
    bool canvasMouseEnabled = false;
    int canvasPenPolygonCount = 3; // Add
    int canvasPenPolygonFill = false; // Add
    QPainter *canvasPainter;
    QPoint canvasPenPointStart;
    QPoint canvasPenPointEnd;
};

#endif // CANVASAREA_H
