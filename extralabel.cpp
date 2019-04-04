#include "extralabel.h"
using namespace std;
ExtraLabel::ExtraLabel(QWidget* parent):QLabel(parent)
{
    mPix = QPixmap();
    setFocusPolicy(Qt::StrongFocus);
    r=15.0;
    mousePressed = false;
    lineSet = false;
    xSizeSet=false;
    rectSet=false;
    ready=false;
    offset=QPoint(0, 0);
    marker = MarkerType::RotationalMarker;

}

void ExtraLabel::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Return|| event->key()==Qt::Key_Enter)
    {
        if(xSizeSet||lineSet)
        {
            ready=true;
            mousePressed=false;
            xSizeSet=false;
            lineSet=false;
            save();
            update();
        }

    }
    else if(event->key()==16777236)//right
    {
        offset+=QPoint(1, 0);
        update();
    }
    else if(event->key()==16777234)//left
    {
        offset+=QPoint(-1, 0);
        update();
    }
    else if(event->key()==16777235)//up
    {
        offset+=QPoint(0, -1);
        update();
    }
    else if(event->key()==16777237)//down
    {
        offset+=QPoint(0, 1);
        update();
    }

}
void ExtraLabel::wheelEvent(QWheelEvent *event)
{
if(event->delta()>0)
{
    r=r*scaling_factor;
}
else
{
    r=r*1/scaling_factor;

}

update();
}
void ExtraLabel::mousePressEvent(QMouseEvent* event){

    if(marker==RotationalMarker)
    {
        if(event->button()==Qt::LeftButton)
     {
         mousePressed = true;
         offset=QPoint(0, 0);
         mLine.setP1(event->pos());
         mLine.setP2(event->pos());
      }
   else if (event->button()==Qt::RightButton)
        {
        r=sqrt(mLine.dx()*mLine.dx()+mLine.dy()*mLine.dy())/2;
        xSizeSet=true;
        lineSet=false;
       // offset=QPoint(0, 0);

    }

    }


    else if(marker==RectangleMarker)
    {
     if(event->button()==Qt::LeftButton)
     {
        mousePressed = true;

        mRect.setTopLeft(event->pos());
        mRect.setBottomRight(event->pos());
       }
    }
}

void ExtraLabel::mouseMoveEvent(QMouseEvent* event){

    if(marker==RotationalMarker)
    {
    if(event->type() == QEvent::MouseMove){
        if(mousePressed)  mLine.setP2(event->pos());


    }
}
    else if(marker==RectangleMarker)
    {
        if(event->type()==QEvent::MouseMove)
        {
            if(mousePressed) mRect.setBottomRight(event->pos());
        }
    }

    this->update();

}

void ExtraLabel::mouseReleaseEvent(QMouseEvent *event){


  if(event->button()==Qt::LeftButton)  mousePressed = false;

    update();

}

void ExtraLabel::save()
{
    /*
        qDebug()<<mLine.dx();
          qDebug()<<mLine.dy();
    double angle=qAtan((float)mLine.dy()/mLine.dx());
    if(angle<0)angle=angle+M_PI;
    angle=-angle;
    QRect boundingRect = pol.boundingRect();
    QImage original = pixmap()->copy(boundingRect).toImage();
    QImage map = QImage(boundingRect.size(), QImage::Format_RGB16);
    map.fill(Qt::black);
    QPoint center(map.width()/2,map.height()/2);

    for(int x=0; x<boundingRect.width(); x++)
    {
        for(int y=0; y<boundingRect.height(); y++)
        {
            if(pol.containsPoint(boundingRect.topLeft()+QPoint(x, y), Qt::OddEvenFill))
            {
                int modx = x;
                int mody=y;
                float s = sin(angle);
                float c = cos(angle);
                modx-=center.x();
                mody-=center.y();
                float xnew = modx * c - mody * s;
                float ynew = modx * s + mody * c;
                modx=xnew+center.x();
                mody=ynew+center.y();
                map.setPixel(modx, mody, original.pixel(x, y) );
            }
        }
    }
*/

QPoint side1 = pol.at(1)-pol.at(0);
QPoint side2 = pol.at(2)-pol.at(1);
int width = sqrt(side1.x()*side1.x()+side1.y()*side1.y());
int height = sqrt(side2.x()*side2.x()+side2.y()*side2.y());

QImage image = QImage(width, height, QImage::Format_RGB16);
image.fill(Qt::black);
QRect boundingRect = pol.boundingRect();
QImage original = pixmap()->copy(boundingRect).toImage();
int offset_x, offset_y;
double angle=qAtan((float)mLine.dy()/mLine.dx());


if(angle<0)angle=angle+M_PI;

QPoint center = QPoint(width/2,height/2);
offset_x=(boundingRect.width()-width)/2;
offset_y =(boundingRect.height()-height)/2;
float s = sin(angle+M_PI);
float c = cos(angle+M_PI);
for(int x = 0; x< width; x++)
{
    for(int y = 0; y<height; y++)
    {
        int modx = x;
        int mody=y;

        modx-=center.x();
        mody-=center.y();
        float xnew = modx * c - mody * s;
        float ynew = modx * s + mody * c;
        modx=xnew+center.x()+offset_x;
        mody=ynew+center.y()+offset_y;
        image.setPixel(x, y, original.pixel(modx, mody) );

    }



}
QString cords;
cords+= QString::number(cLine.center().x());
cords+=";";
cords+=QString::number(cLine.center().y());
cords+=";";
cords+=QString::number(width);
cords+=";";
cords+=QString::number(height);
cords+=";";
cords+=QString::number(angle);
cords+=";";
QMessageBox box;


    box.setIconPixmap(QPixmap::fromImage(image));

    QAbstractButton* pSaveButton = box.addButton(tr("Save"), QMessageBox::YesRole);
    box.addButton(tr("Don't save"), QMessageBox::NoRole);
    box.exec();
    if (box.clickedButton()==pSaveButton) {
       emit imageToBeSaved(image, cords);
    }


}




void ExtraLabel::paintEvent(QPaintEvent *event)
{
    if(marker==RotationalMarker)
    {
    QLabel::paintEvent(event);
    painter.begin(this);
    QPen pen(Qt::red, 5);
    painter.setPen(pen);

    if(mousePressed){
       /*
        painter.drawPixmap(0,0,mPix);

       painter.drawRect(mRect);


        */
        lineSet= true;
        painter.drawLine(mLine);
    }
    else if (lineSet){
/*
        mPix=*pixmap();
        QPainter tempPainter(&mPix);


        tempPainter.drawRect(mRect);

        painter.drawPixmap(0,0,mPix);
        setPixmap(mPix);
        */


       // mPix=*pixmap();
       // QPainter tempPainter(&mPix);
    /*    qreal angle =qAtan((mLine.y1()-mLine.y2())/(mLine.x2()-mLine.x1()));

        QPoint center = mLine.center();
        QTransform transform = QTransform().translate( center.x(), center.y() ).rotateRadians( 1/angle ).translate( -center.x(), -center.y() );
        QRect rect=QRect(mLine.p1(), mLine.p2());
        QPolygon pol = transform.mapToPolygon(rect);
        */
        QPoint marginRight =QPoint(-r*mLine.dy()/sqrt(mLine.dx()*mLine.dx()+mLine.dy()*mLine.dy()), r*mLine.dx()/sqrt(mLine.dx()*mLine.dx()+mLine.dy()*mLine.dy()));
        QPoint marginLeft= QPoint(-marginRight.x(), -marginRight.y());
        QLine l1 = mLine.translated(marginRight);
        QLine l2 = mLine.translated(marginLeft);

        QVector<QPoint> ver;

        ver.push_back(l1.p1()+offset);
         ver.push_back(l1.p2()+offset);
        ver.push_back(l2.p2()+offset);
        ver.push_back(l2.p1()+offset);



         pol = QPolygon(ver);
       // tempPainter.drawPolygon(pol);
      //  painter.drawPixmap(0,0,mPix);
     //   setPixmap(mPix);
        painter.drawPolygon(pol);
        cLine = QLine(pol.at(0)+(pol.at(1)-pol.at(0))/2-offset,pol.at(2)+ (pol.at(3)-pol.at(2))/2-offset);


    }
    else if(xSizeSet)
    {

       // QPainter tempPainter(&mPix);
        QPoint marginRight =QPoint(r*(mLine.p2()-mLine.p1())/sqrt(mLine.dx()*mLine.dx()+mLine.dy()*mLine.dy()));
        QPoint marginLeft= QPoint(r*(mLine.p1()-mLine.p2())/sqrt(mLine.dx()*mLine.dx()+mLine.dy()*mLine.dy()));

        QLine l1 = cLine.translated(marginRight);
        QLine l2 = cLine.translated(marginLeft);

        QVector<QPoint> ver;


        ver.push_back(l1.p2()+offset);
        ver.push_back(l2.p2()+offset);
        ver.push_back(l2.p1()+offset);
        ver.push_back(l1.p1()+offset);
        pol = QPolygon(ver);
      // tempPainter.drawPolygon(pol);
        painter.drawPolygon(pol);
       }

    painter.end();
    }

    else if(marker==RectangleMarker)
    {
        QLabel::paintEvent(event);
        painter.begin(this);
        QPen pen(Qt::black, 5);
        painter.setPen(pen);

        if(mousePressed){

           // painter.drawPixmap(0,0,mPix);

            painter.drawRect(mRect);



        }

        if(rectSet)
        {
            painter.drawRect(mRect);
        }
        painter.end();
    }
}
void ExtraLabel::resetPainting()
{
    painter.end();
}

void ExtraLabel::setMarker(MarkerType marker)
{
    this->marker = marker;
    resetPainting();
}



