#include "brush.h"
#include "qapplication.h"
#include <iostream>
Brush::Brush()
{
    int type = 0; //0 -> Pencil; 1 -> Rectangle; 2 -> Circle; 3 -> Triangle
    bool isFill = false;
    setColor();
    setSecondColor();
    switchSwap();
    //brushPen = new QPen(Qt::black);
    brushPen = new QPen(brushColor);
    //brushPen->setWidth(4);
    fillBrush = new QBrush(brushSecondColor);
    fillBrush->setStyle(Qt::SolidPattern);
}

void Brush::paint(QPoint &pBegin, QPoint &pEnd, QPainter &painter, int type) {
    if (isSwap) {
        QColor temp = brushPen->color();
        brushPen->setColor(fillBrush->color());
        fillBrush->setColor(temp);
    }
    painter.setPen(*brushPen);
    //std::cout << "ISFILL = " << isFill << std::endl;
    if (type != 0) {
        if (QGuiApplication::queryKeyboardModifiers() == Qt::ShiftModifier) { //! Shift functionality
            int diff = std::min(abs(pEnd.x() - pBegin.x()), abs(pEnd.y() - pBegin.y()));
            if (pEnd.x() > pBegin.x()) {
                pEnd.setX(pBegin.x()+diff);
            } else {
                pEnd.setX(pBegin.x()-diff);
            }
            if (pEnd.y() > pBegin.y()) {
                pEnd.setY(pBegin.y()+diff);
            } else {
                pEnd.setY(pBegin.y()-diff);
            }
        }

        if (type == 1) {
            QRect rect = QRect(pBegin, pEnd);
            QPainterPath path;
            path.addRect(rect);
            //painter.drawRect(rect);
            if (isFill) {
                //painter.setBrush(*fillBrush);
                //painter.drawRect(rect);
                //painter.setBrush(*brushPen);
                //painter.fillRect(rect, *fillBrush);
                painter.fillPath(path, *fillBrush);
            }
            painter.drawPath(path);
        } else if (type == 2) {
            QRect circle = QRect(pBegin, pEnd);
            QPainterPath path;
            path.addEllipse(circle);
            //painter.drawEllipse(circle);
            if (isFill) {
                //painter.setBrush(*fillBrush);
                //painter.drawEllipse(circle);
                //painter.setBrush(*brushPen);
                painter.fillPath(path, *fillBrush);
            }
            painter.drawPath(path);
        } else if (type == 3) {
            QPolygon triangle;
            triangle << QPoint(pBegin.x(), pBegin.y()) << QPoint(pBegin.x(), pEnd.y()) << QPoint(pEnd);
            QPainterPath path;
            path.addPolygon(triangle);
            if (isFill) {
                //painter.setBrush(*fillBrush);
                //painter.drawPolygon(triangle);
                //painter.setBrush(*brushPen);
                painter.fillPath(path, *fillBrush);
            }
            painter.drawPolygon(triangle);
            //painter.drawPath(path);
        }
    } else {
        painter.drawLine(pBegin, pEnd);
        pBegin = pEnd;
    }
    if (isSwap) {
        QColor temp = brushPen->color();
        brushPen->setColor(fillBrush->color());
        fillBrush->setColor(temp);
    }
}

