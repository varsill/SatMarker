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
#include "toollabel.h"
class RotationalLabel : public ToolLabel
{
    Q_OBJECT
public:
    ExtraLabel(QWidget* parent = 0);

private:

   double scaling_factor = 11/10.0;
   double range;

   QPolygon pol;
   QLine cLine;
   QLine mLine;
   QPoint offset;

//states:
   bool lineSet;
   bool mousePressed;
   bool xSizeSet;
   bool ready;
};

#endif // EXTRALABEL_H
