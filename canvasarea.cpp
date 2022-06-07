#include "canvasarea.h"
#include <math.h>
#include <iostream>
#include <queue>
CanvasArea::CanvasArea(QWidget *parent)
    : QWidget{parent}
{
    //QPainter painter(this);
    QString fileName = QFileDialog::getOpenFileName(this, //Move this to separate function
                                    tr("Open Image"),
                                    QDir::currentPath(),
                                    tr("Image Files (*.png *.jpg *.bmp)"));
    canvasImage = new QImage(fileName);
    //*canvasImage = canvasImage->scaled(canvasImage->size()/5, Qt::KeepAspectRatio); // Use to scale //To export bring to default size // Multiply pen by scale
    setMinimumSize(canvasImage->size());
    canvasMode = 9; // Remove this, handle through
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
    int rad = (painter->pen().width() / 2) + 4;
    painter->setPen(QPen(canvasPrimaryColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); // TODO More Variables
    if (canvasCustomShapePolygon.empty()) {
        canvasCustomShapePolygon << canvasPenPointStart << canvasPenPointEnd;
    } else {
        if (abs(canvasCustomShapePolygon.point(0).x() - canvasPenPointEnd.x()) < 3 && abs(canvasCustomShapePolygon.point(0).y() - canvasPenPointEnd.y()) < 3 && canvasMouseReleased) {
            canvasCustomShapePolygonLooped = true;
        } else {
            canvasCustomShapePolygon << canvasPenPointEnd;
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
}

void CanvasArea::canvasSelectRectangleArea(QPainter *painter) {
    const int scale = 3;
    QPen pen;
    pen.setWidth(1);

    QPolygon polygon;
    polygon << canvasPenPointStart << QPoint(canvasPenPointStart.x(), canvasPenPointEnd.y()) << canvasPenPointEnd << QPoint(canvasPenPointEnd.x(), canvasPenPointStart.y());

    if (canvasMouseReleased) {
        canvasMode = 8;
        canvasImageCopyPoint = polygon.boundingRect().topLeft();//canvasPenPointStart;
        canvasImageCopyBuffer = canvasImage->copy(polygon.boundingRect());
    }

    pen.setColor(Qt::white);
    pen.setDashPattern({1.0 * scale, 1.0 * scale});
    painter->setPen(pen); // TODO More Variables

    painter->drawPolygon(polygon, Qt::OddEvenFill);

    pen.setColor(Qt::black);
    pen.setDashOffset(1.0 * scale);
    painter->setPen(pen); // TODO More Variables

    painter->drawPolygon(polygon, Qt::OddEvenFill);

}

void CanvasArea::canvasMoveBufferImage(QPainter *painter) {
    int offsetx = canvasPenPointStart.x() - canvasImageCopyPoint.x();
    int offsety = canvasPenPointStart.y() - canvasImageCopyPoint.y();

    if (offsetx < 0 || offsetx > canvasImageCopyBuffer.width() || offsety < 0 || offsety > canvasImageCopyBuffer.height() ) {
        //std::cout << "Offset => " << offsetx << " " << offsety << std::endl;
        //std::cout << "Width = " << canvasImageCopyBuffer.width() << "\nHeight = " << canvasImageCopyBuffer.height() << std::endl;
        canvasMode = 7;
    }
    painter->drawImage(canvasImageCopyPoint + canvasPenPointEnd - canvasPenPointStart, canvasImageCopyBuffer);

    QPen pen;
    pen.setWidth(1);

    QPolygon polygon;
    pen.setColor(Qt::white);
    pen.setDashPattern({3.0, 3.0});
    painter->setPen(pen); // TODO More Variables

    painter->drawRect(canvasImageCopyPoint.x() + canvasPenPointEnd.x() - canvasPenPointStart.x(), canvasImageCopyPoint.y() + canvasPenPointEnd.y() - canvasPenPointStart.y(),
                      canvasImageCopyBuffer.width(), canvasImageCopyBuffer.height());

    pen.setDashOffset(3.0);
    pen.setColor(Qt::black);
    painter->drawRect(canvasImageCopyPoint.x() + canvasPenPointEnd.x() - canvasPenPointStart.x(), canvasImageCopyPoint.y() + canvasPenPointEnd.y() - canvasPenPointStart.y(),
                      canvasImageCopyBuffer.width(), canvasImageCopyBuffer.height());

    if (canvasMouseReleased) {
        canvasImageCopyPoint += canvasPenPointEnd - canvasPenPointStart;
    }
}

bool CanvasArea::validPoint(QPoint point) {
    return point.x() >= 0 && point.y() >= 0 && point.x() < canvasImage->width() && point.x() < canvasImage->height();
}

void CanvasArea::canvasDrawBucket(QPainter *painter) {
    if (!canvasMouseReleased) return;
    std::queue<QPoint> points;
    points.push(canvasPenPointEnd);

    QRgb oldColor = canvasImage->pixel(canvasPenPointEnd);
    QRgb newColor = canvasPrimaryColor.rgb();

    if (oldColor == newColor) {
        return;
    }
    std::cout << "Width = " << canvasImage->width() << " Height = " << canvasImage->height() << std::endl;

    while (!points.empty()) {
        QPoint origin = points.front();
        points.pop();
        int lx = origin.x() - 1;
        int rx = origin.x();
        while (lx >= 0 && canvasImage->pixel(QPoint(lx, origin.y())) == oldColor) {
            canvasImage->setPixelColor(QPoint(lx, origin.y()), newColor);
            lx--;
        }


        while (rx < canvasImage->width() && canvasImage->pixel(QPoint(rx, origin.y())) == oldColor) {
            canvasImage->setPixelColor(QPoint(rx, origin.y()), newColor);
            rx++;
        }

        bool added = false;
        if (origin.y()>0) {
            for (int i = lx+1; i < rx; i++) {
                if (canvasImage->pixel(QPoint(i, origin.y()-1)) != oldColor) {
                    added = false;
                } else if (!added) {
                    points.push(QPoint(i, origin.y()-1));
                }
            }
        }
        if (origin.y()+1 < canvasImage->height()) {
            added = false;
            for (int i = lx+1; i < rx; i++) {
                if (canvasImage->pixel(QPoint(i, origin.y()+1)) != oldColor) {
                    added = false;
                } else if (!added) {
                    points.push(QPoint(i, origin.y()+1));
                }
            }
        }
    }
}

void CanvasArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    //painter.scale(2,2);

    //setMinimumSize(canvasImage->size());
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
        case 7: if (canvasMouseReleased) {
                    canvasSelectRectangleArea(&painter);
                } else canvasSelectRectangleArea(&painter);
                rad = (painter.pen().width() / 2) + 4;
                //update(QRect(canvasPenPointStart, canvasPenPointEnd).normalized().adjusted(-rad, -rad, +rad, +rad));
                break;
        case 8: if (canvasMouseReleased) {
                    canvasMoveBufferImage(&painter);
                } else canvasMoveBufferImage(&painter);
                //rad = (painter.pen().width() / 2) + 4;
                if (canvasMode == 7) {
                    //canvasImageCopyBuffer.fill(Qt::black);
                    canvasPainter->drawImage(canvasImageCopyPoint + canvasPenPointEnd - canvasPenPointStart, canvasImageCopyBuffer);
                    update(canvasImageCopyBuffer.rect());
                    //update(QRect(canvasImageCopyPoint, QPoint(canvasImageCopyPoint.x() + canvasImageCopyBuffer.width(), canvasImageCopyPoint.y() + canvasImageCopyBuffer.height())).normalized().adjusted(-rad, -rad, +rad, +rad));
                }
                break;
        case 9:
                canvasDrawBucket(canvasPainter);
                update();
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




void CanvasArea::setMode(int value) {
    std::cout << "CHANGED MODE TO " << value << std::endl;
    if (value != canvasMode) {
        canvasMode = value;
        //update();
        emit canvasModeChanged(value); // Needs a *clear all* function
        canvasCustomShapePolygon.clear();
        canvasCustomShapePolygonLooped = false;
        canvasCurvePath->clear();
        canvasCurvePathPoints = 0;
        update();
    }
}

void CanvasArea::setColor(int i) {
    if (i == 0) {
        QColor newColor = QColorDialog::getColor(canvasPrimaryColor, parentWidget());
        if (newColor != canvasPrimaryColor) canvasPrimaryColor = newColor;
    } else if (i == 1) {
        QColor newColor = QColorDialog::getColor(canvasSecondaryColor, parentWidget());
        if (newColor != canvasSecondaryColor) canvasSecondaryColor = newColor;
    }
}

void CanvasArea::setBrushSize(int i) {
    if (i != canvasBrushSize) {
        std::cout << "New Brush Size = " << i << std::endl;
        canvasBrushSize = i;
        emit canvasBrushChanged(i);
    }
}

void CanvasArea::changeFill() {
    canvasFill = !canvasFill;
}
