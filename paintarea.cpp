#include "paintarea.h"
#include <iostream>
PaintArea::PaintArea(QWidget *parent)
    : QWidget{parent}
{
    start();
}

void PaintArea::start() {
    //mImage = new QImage(width(), height(), QImage::Format_ARGB32_Premultiplied);
    mImage = new QImage(800, 800, QImage::Format_ARGB32_Premultiplied);
    std::cout << height() << std::endl;
    mImage->fill(Qt::white);
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/path/to/file/",tr("Images (*.png *.xpm *.jpg)"));
    //mImage = new QImage(fileName);
    mPainter = new QPainter(mImage);
    mEnabled = false;
    mStopped = false;
    mPen = new QPen(Qt::black);
    mPen->setCapStyle(Qt::RoundCap);
    mPen->setWidth(5);
    mBrushType = 0;
    bufferInteract = false;
    //mPainter->setPen(*mPen);
}

void PaintArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, *mImage);
    if (bufferInteract) painter.drawImage(mEnd.x(), mEnd.y(), *bufferImage);
    //mPainter->drawImage(0, 0, *mImage);
    update();

    if (mEnabled) {
        QPainter overlay(this);
        if (mBrushType == 0) mBrush.paint(mBegin, mEnd, *mPainter, mBrushType);
        else if (mBrushType == 4) { // Moving
            if (!bufferInteract) {
                mBrush.paint(mBegin, mEnd, painter, 1);
            } else {
                painter.drawImage(mEnd.x(), mEnd.y(), *bufferImage);
            }
        }
        else mBrush.paint(mBegin, mEnd, painter, mBrushType);
        mStopped = true;
    } else if (mStopped && mBrushType != 0) {
        if (mBrushType == 4) { // Moving
            if (!bufferInteract) {
                bufferInteract = true;
                mBrush.paint(mBegin, mEnd, painter, 1); // Does shift work?
                bufferImage = new QImage(abs(mEnd.x() - mBegin.x()), abs(mEnd.y() - mBegin.y()), QImage::Format_ARGB32_Premultiplied);
                *bufferImage = mImage->copy(mBegin.x(), mBegin.y(), abs(mEnd.x() - mBegin.x()), abs(mEnd.y() - mBegin.y()));
                bufferImage->save("bufferImage.png");
                //bufferImage->fill(Qt::white);
                mPainter->drawImage(mBegin.x(), mBegin.y(), *bufferImage);
                buffermBegin = mBegin;
                bufferHeight = abs(mEnd.y() - mBegin.y());
                bufferWidth = abs(mEnd.x() - mBegin.x());
            } else {
                if (abs(buffermBegin.x()-bufferOffset.x()) > bufferWidth || abs(buffermBegin.y()-bufferOffset.y()) > bufferHeight) {
                    bufferInteract = false;
                    mPainter->drawImage(mEnd.x(), mEnd.y(), *bufferImage);
                    std::cout << "Missed" << std::endl;
                }
                else {
                    bufferOffsetX = buffermBegin.x() - mEnd.x();
                    bufferOffsetY = buffermBegin.y() - mEnd.y();
                    QImage temp = mImage->copy(mEnd.x(), mEnd.y(), bufferWidth, bufferHeight);
                    mPainter->drawImage(mEnd.x(), mEnd.y(), *bufferImage);
                    std::cout << "In place" << std::endl;
                }
            }
        } else mBrush.paint(mBegin, mEnd, *mPainter, mBrushType);
        mStopped = false;
    }
    painter.end();
}

void PaintArea::mouseMoveEvent(QMouseEvent *event) {
    //if (event->button() != Qt::RightButton && event->button() != Qt::LeftButton) return;
    if (!mEnabled) {
        event->accept();
        return;
    }
    mEnd = event->pos();
    update();
}

void PaintArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        mBrushType = (mBrushType+1)%5;
        return;
    } else if (event->button() == Qt::XButton1) {
        mImage->save("export.png");
        return;
    }


    if (event->button() == Qt::RightButton) {
        mBrush.switchSwap(true);
    } else if (event->button() == Qt::LeftButton) {
        if (mBrushType == 4) {
            bufferOffset = event->pos();
        }
        else mBrush.switchSwap();
    }
    //if (event->button() == Qt::LeftButton) {
    mEnabled = true;
    mBegin = event->pos();
    mEnd = event->pos();
    event->accept();
    //}
    update();
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::RightButton && event->button() != Qt::LeftButton) return;

    if (event->button() == Qt::RightButton) {
        mBrush.switchSwap(true);
    } else if (event->button() == Qt::LeftButton) {
        mBrush.switchSwap();
    }
    mEnabled = false;
    event->accept();
    //}

    update();
}
