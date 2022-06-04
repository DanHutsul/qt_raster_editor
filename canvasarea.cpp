#include "canvasarea.h"
#include <math.h>
#include <iostream>
CanvasArea::CanvasArea(QWidget *parent)
    : QWidget{parent}
{
    //QPainter painter(this);
    QString fileName = QFileDialog::getOpenFileName(this, //Move this to separate function
                                    tr("Open Image"),
                                    QDir::currentPath(),
                                    tr("Image Files (*.png *.jpg *.bmp)"));
    canvasImage = new QImage(fileName);
    setMinimumSize(canvasImage->size());
    canvasMode = 6; // Remove this, handle through
    canvasPrimaryColor = Qt::black;
    canvasSecondaryColor = Qt::white;
    canvasPainter = new QPainter(canvasImage);
    canvasMouseReleased = false;
    canvasCurvePath = new QPainterPath();
    //canvasPainter->setPen(QPen(canvasPrimaryColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); // TODO More Variables
}

void CanvasArea::canvasDrawPencil(QPainter *painter) {
    painter->setPen(QPen(canvasPrimaryColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); // TODO More Variables
    painter->drawLine(canvasPenPointStart, canvasPenPointEnd);
    //int rad = (painter->pen().width() / 2) + 2;
    canvasPenPointStart = canvasPenPointEnd;
    //update();
    //update(QRect(canvasPenPointStart, canvasPenPointEnd).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void CanvasArea::canvasDrawEraser(QPainter *painter) {
    painter->setPen(QPen(canvasSecondaryColor, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); // TODO More Variables
    painter->drawLine(canvasPenPointStart, canvasPenPointEnd);
    canvasPenPointStart = canvasPenPointEnd;
}

void CanvasArea::canvasDrawLine(QPainter *painter) {
    painter->setPen(QPen(canvasPrimaryColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(canvasPenPointStart, canvasPenPointEnd);
}

void CanvasArea::canvasDrawCurve(QPainter *painter) { //TODO clear path
    if (canvasCurvePath->isEmpty()) {
        canvasCurvePathStart = canvasPenPointStart;
        canvasCurvePathEnd = canvasPenPointEnd;
        canvasCurvePath->moveTo(canvasCurvePathStart);
        canvasCurvePath->lineTo(canvasCurvePathEnd);
        canvasCurvePathPoints = 2;
    } else {
        if (canvasCurvePathPoints == 2) {
            canvasCurvePath->clear();
            canvasCurvePathStart = canvasPenPointStart;
            canvasCurvePathEnd = canvasPenPointEnd;
            canvasCurvePath->moveTo(canvasCurvePathStart);
            canvasCurvePath->lineTo(canvasCurvePathEnd);
        }
        else if (canvasCurvePathPoints == 3) {
            canvasCurvePathMiddle1 = canvasPenPointEnd;
            canvasCurvePath->clear();
            canvasCurvePath->moveTo(canvasCurvePathStart);
            canvasCurvePath->quadTo(canvasCurvePathMiddle1, canvasCurvePathEnd);
        }
        else if (canvasCurvePathPoints == 4) {
            canvasCurvePathMiddle2 = canvasPenPointEnd;
            canvasCurvePath->clear();
            canvasCurvePath->moveTo(canvasCurvePathStart);
            canvasCurvePath->cubicTo(canvasCurvePathMiddle1, canvasCurvePathMiddle2, canvasCurvePathEnd);
        }
        else if (canvasCurvePathPoints == 5) {
            canvasCurvePath->clear();
            canvasCurvePathPoints = 0;
            return;
        }
    }
    if (canvasMouseReleased) canvasCurvePathPoints++;
    painter->setPen(QPen(canvasPrimaryColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(*canvasCurvePath);
}

void CanvasArea::canvasDrawRectangle(QPainter *painter) {
    painter->setPen(QPen(canvasPrimaryColor, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); // TODO More Variables
    QPolygon polygon;
    polygon << canvasPenPointStart << QPoint(canvasPenPointStart.x(), canvasPenPointEnd.y()) << canvasPenPointEnd << QPoint(canvasPenPointEnd.x(), canvasPenPointStart.y());
    painter->drawPolygon(polygon, Qt::OddEvenFill); // Add fill options
}

void CanvasArea::canvasDrawCustomShape(QPainter *painter) {
    if (!canvasMouseReleased) {
        if (canvasCustomShapePolygon.count() == 2) {
            canvasCustomShapePolygon.clear();
            canvasCustomShapePolygonLooped = false;
        } else if (!canvasCustomShapePolygon.isEmpty()) canvasCustomShapePolygon.pop_back();
        else canvasCustomShapePolygonLooped = false;
    }
    //bool looped = false;
    int rad = (painter->pen().width() / 2) + 4;
    painter->setPen(QPen(canvasPrimaryColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); // TODO More Variables
    if (canvasCustomShapePolygon.empty()) {
        canvasCustomShapePolygon << canvasPenPointStart << canvasPenPointEnd;
        //painter->drawLine(canvasCustomShapePolygon.point(canvasCustomShapePolygon.count()-2), canvasCustomShapePolygon.point(canvasCustomShapePolygon.count()-1));
        //if (!canvasMouseReleased) canvasCustomShapePolygon.clear();
    } else {
        if (abs(canvasCustomShapePolygon.point(0).x() - canvasPenPointEnd.x()) < 3 && abs(canvasCustomShapePolygon.point(0).y() - canvasPenPointEnd.y()) < 3 && canvasMouseReleased) {
            canvasCustomShapePolygonLooped = true;
            //canvasPainter->drawPolygon(canvasCustomShapePolygon, Qt::OddEvenFill); // Add fill options
            //update(canvasCustomShapePolygon.boundingRect().normalized().adjusted(-rad, -rad, +rad, +rad));
            //canvasCustomShapePolygon.clear();
        } else {
            canvasCustomShapePolygon << canvasPenPointEnd;
            /*for (int i = 1; i < canvasCustomShapePolygon.count(); i++) {
                painter->drawLine(canvasCustomShapePolygon.point(i-1), canvasCustomShapePolygon.point(i));
            }*/
            //if (!canvasMouseReleased) canvasCustomShapePolygon.pop_back();
        }
    }


    if (canvasCustomShapePolygonLooped) {
        canvasPainter->drawPolygon(canvasCustomShapePolygon, Qt::OddEvenFill);
        update(canvasCustomShapePolygon.boundingRect().normalized().adjusted(-rad, -rad, +rad, +rad));
        canvasCustomShapePolygon.clear();
        //canvasCustomShapePolygonLooped = false;
    } else {
        for (int i = 1; i < canvasCustomShapePolygon.count(); i++) {
            painter->drawLine(canvasCustomShapePolygon.point(i-1), canvasCustomShapePolygon.point(i));
        }
    }
    //update(canvasCustomShapePolygon.boundingRect().normalized().adjusted(-rad, -rad, +rad, +rad));
    /*f (!canvasMouseReleased) {
        if (canvasCustomShapePolygon.count() == 2) canvasCustomShapePolygon.clear();
        else canvasCustomShapePolygon.pop_back();
    }*/
}

void CanvasArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawImage(0, 0, *canvasImage);
    painter.setPen(QPen(canvasPrimaryColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    int rad;
    if (canvasMouseEnabled) {
        switch(canvasMode) {
        case 1: canvasDrawPencil(canvasPainter); rad = (painter.pen().width() / 2) + 4; update(QRect(canvasPenPointStart, canvasPenPointEnd).normalized().adjusted(-rad, -rad, +rad, +rad)); break; // Pencil
        case 2: canvasDrawEraser(canvasPainter); rad = (painter.pen().width() / 2) + 4; update(QRect(canvasPenPointStart, canvasPenPointEnd).normalized().adjusted(-rad, -rad, +rad, +rad)); break; // Eraser
        case 3: if (canvasMouseReleased) canvasDrawLine(canvasPainter); //Line
                else canvasDrawLine(&painter);
                rad = (painter.pen().width() / 2) + 4;
                update(QRect(canvasPenPointStart, canvasPenPointEnd).normalized().adjusted(-rad, -rad, +rad, +rad));
                break;
        case 4: if (canvasMouseReleased && canvasCurvePathPoints == 4) {
                    canvasDrawCurve(canvasPainter);
                    rad = (painter.pen().width() / 2) + 4;
                    update(canvasCurvePath->boundingRect().toRect().normalized().adjusted(-rad, -rad, +rad, +rad));
                }
                else canvasDrawCurve(&painter);
                break;
        case 5: if (canvasMouseReleased) {
                    canvasDrawRectangle(canvasPainter);
                } else canvasDrawRectangle(&painter);
                rad = (painter.pen().width() / 2) + 4;
                update(QRect(canvasPenPointStart, canvasPenPointEnd).normalized().adjusted(-rad, -rad, +rad, +rad));
                break;
        case 6: if (canvasCustomShapePolygonLooped && canvasMouseReleased) {
                    canvasDrawCustomShape(canvasPainter);
                    //update(canvasCustomShapePolygon.boundingRect().normalized().adjusted(-rad, -rad, +rad, +rad));
                } else canvasDrawCustomShape(&painter);

                break;
        }


        /*canvasPainter->drawLine(canvasPenPointStart, canvasPenPointEnd);
        canvasPenPointStart = canvasPenPointEnd;*/
    }
    if (canvasMouseReleased) canvasMouseEnabled = false;
    //update(QRect(canvasPenPointStart, canvasPenPointEnd).normalized().adjusted(-rad, -rad, +rad, +rad));
    //update();
    //painter.drawImage(event->rect(), *canvasImage, event->rect());
}

void CanvasArea::mousePressEvent(QMouseEvent* event) {
    canvasMouseReleased = false;
    canvasMouseEnabled = true;
    canvasPenPointStart = event->pos();
    canvasPenPointEnd = event->pos();
    //event->accept();
    update();
}

void CanvasArea::mouseMoveEvent(QMouseEvent* event) {
    //canvasPenPointStart = canvasPenPointEnd;
    canvasPenPointEnd = event->pos();
    //event->accept();
    update();
}

void CanvasArea::mouseReleaseEvent(QMouseEvent* event) {
    //canvasPenPointStart = canvasPenPointEnd;
    canvasMouseReleased = true;
    canvasPenPointEnd = event->pos();
    //event->accept();
    update();
}
