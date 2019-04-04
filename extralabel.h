#ifndef EXTRALABEL_H
#define EXTRALABEL_H
#include <QLabel>
#include <QRect>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include <QLine>
#include <QPoint>
#include <QtMath>
#include <QMessageBox>
#include <QVector>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include "markertype.h"
class ExtraLabel : public QLabel
{
    Q_OBJECT
public:
    ExtraLabel(QWidget* parent = 0);
private:
   double scaling_factor = 11/10.0;
   QPixmap mPix;
   bool lineSet;
   bool mousePressed;
   bool rectSet;
   bool xSizeSet;
   bool ready;
   QRect mRect;
   QPolygon pol;
   QPainter painter;
   QLine cLine;
   QLine mLine;
   QPoint offset;
   MarkerType marker;
   double r;
signals:
   void imageToBeSaved(QImage, QString);
private:
   void save();
   void mousePressEvent(QMouseEvent*);
   void mouseMoveEvent(QMouseEvent*);
   void mouseReleaseEvent(QMouseEvent*);
   void paintEvent(QPaintEvent*) ;
   void wheelEvent(QWheelEvent*);
   void keyPressEvent(QKeyEvent*);

   void resetPainting();
   template <class T, class ... R> static QString toCSV(T arg, R...rest)
    {
         return QString(arg)+";"+toCSV(rest...);
    }
   template <class ... R> static QString toCSV(int arg, R...rest)
   {
        return QString::number(arg)+";"+toCSV(rest...);
   }
   template <class ... R> static QString toCSV(QString arg, R...rest)
   {
        return arg+";"+toCSV(rest...);

   }

public:
   void setMarker(MarkerType);
};

#endif // EXTRALABEL_H
