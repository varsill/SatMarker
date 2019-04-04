#include "extrascrollarea.h"

ExtraScrollArea::ExtraScrollArea()
{
        mPix = QPixmap(400,400);
        mPix.fill(Qt::white);

        mousePressed = false;
        drawStarted = false;

}

void ExtraScrollArea::wheelEvent(QWheelEvent* event)
{


}

void ExtraScrollArea::mousePressEvent(QMouseEvent* event){

    mousePressed = true;


    mRect.setTopLeft(event->pos());
    mRect.setBottomRight(event->pos());








}

void ExtraScrollArea::mouseMoveEvent(QMouseEvent* event){


    if(event->type() == QEvent::MouseMove){

        mRect.setBottomRight(event->pos());
    }

    this->update();
    this->repaint();
}

void ExtraScrollArea::mouseReleaseEvent(QMouseEvent *event){

    //When mouse is released update for the one last time
    mousePressed = false;

    update();

}

void ExtraScrollArea::paintEvent(QPaintEvent *event){

    painter.begin(this);


    if(mousePressed){

       painter.drawPixmap(0,0,mPix);

       painter.drawRect(mRect);

        drawStarted = true;
    }
    else if (drawStarted){

        QPainter tempPainter(&mPix);

        tempPainter.drawRect(mRect);

        painter.drawPixmap(0,0,mPix);
    }

    painter.end();
}


