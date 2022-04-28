#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QColor>
#include <QColorDialog>
#include <QGuiApplication>
#include <QFileDialog>
#include <brush.h>
class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = nullptr);
    //~PaintArea();
    void start();
signals:
public slots:

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QLine m_line;
    QPixmap m_nPTargetPixmap;
    QPainter *mPainter;
    QImage *mImage;
    QPoint mBegin;
    QPoint mEnd;
    QPen *mPen;
    bool mEnabled;
    bool mStopped;
    Brush mBrush;
    int mBrushType; //0 -> Pencil; 1 -> Rectangle; 2 -> Circle; 3 -> Triangle; 4 -> Select area

    QImage *bufferImage;
    bool bufferInteract;
    QPoint bufferOffset;
    int bufferOffsetX;
    int bufferOffsetY;
    QPoint buffermBegin;
    int bufferHeight;
    int bufferWidth;
};

#endif // PAINTAREA_H
