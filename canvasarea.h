#ifndef CANVASAREA_H
#define CANVASAREA_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainterPath>
#include <QColorDialog>
class CanvasArea : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasArea(QWidget *parent = nullptr);
    QColor getCanvasPrimaryColor() {return canvasPrimaryColor;}
    QColor getCanvasSecondaryColor() {return canvasSecondaryColor;}
    void setColor(int i);
    void setBrushSize(int i);
public slots:
    void setMode(int value);
    void changeFill();
signals:
    void canvasModeChanged(int newValue);
    void canvasBrushChanged(int newValue);
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
    void canvasSelectRectangleArea(QPainter *painter);
    void canvasMoveBufferImage(QPainter *painter);
    void canvasDrawBucket(QPainter *painter);

    void floodFill(QPoint point, QRgb oldColor, QRgb newColor);

    //void canvasDrawPolygon(QPainter *painter, int n);
    void canvasDrawCircle(bool updateFlag = false);
    bool validPoint(QPoint point);
private:
    //QPainter canvasPainter;
    QImage *canvasImage;
    QImage *canvasImageBackup; // Unused?
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

    int canvasBrushSize;
    int canvasScale;

    QImage canvasImageCopyBuffer;
    QPoint canvasImageCopyPoint;

    bool canvasFill = false;
};

#endif // CANVASAREA_H
