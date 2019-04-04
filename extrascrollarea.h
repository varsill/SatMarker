#ifndef EXTRASCROLLAREA_H
#define EXTRASCROLLAREA_H
#include <QScrollArea>
#include <QWheelEvent>
#include <QPointF>
#include <QScrollBar>
#include <QPixmap>
#include <QRect>
#include <QPainter>
#include <iostream>
using namespace std;
class ExtraScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    ExtraScrollArea();
    void wheelEvent(QWheelEvent*);
signals:
    void wheelUp();
    void wheelDown();
private:
    QPixmap mPix;
    bool drawStarted;
    bool mousePressed;
    QRect mRect;
    QPainter painter;
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*) override;
};

#endif // EXTRASCROLLAREA_H
