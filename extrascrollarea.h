#ifndef RotationalSCROLLAREA_H
#define RotationalSCROLLAREA_H
#include <QScrollArea>
#include <QWheelEvent>
#include <QPointF>
#include <QScrollBar>
#include <QPixmap>
#include <QRect>
#include <QPainter>
#include <iostream>
using namespace std;
class RotationalScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    RotationalScrollArea();
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

#endif // RotationalSCROLLAREA_H
