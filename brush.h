#ifndef BRUSH_H
#define BRUSH_H

#include <QPen>
#include <QPainter>
#include <QPainterPath>

class Brush
{
public:
    Brush();
    //~Brush();
    void paint(QPoint &pBegin, QPoint &pEnd, QPainter &painter, int type);
    void setFill(bool val = false) {isFill = val;}
    void setColor(QColor newColor = Qt::black) {
        brushColor = newColor;
    }
    void setSecondColor(QColor newColor = Qt::white) {
        brushSecondColor = newColor;
    }
    void switchSwap(bool val=false) {
        isSwap = val;
    }
private:
    QPen *brushPen;
    QBrush *fillBrush;
    QColor brushColor;
    //int type; //0 -> Pencil; 1 -> Rectangle; 2 -> Circle; 3 -> Triangle
    bool isFill = true;
    bool isSwap;
    QColor brushSecondColor;
};

#endif // BRUSH_H
