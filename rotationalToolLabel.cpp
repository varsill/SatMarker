#include "rotationalToolLabel.h"
using namespace std;

RotationalLabel::RotationalLabel(QWidget* parent):QLabel(parent)
{

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


void RotationalLabel::keyPressEvent(QKeyEvent *event)
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


void RotationalLabel::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
       range=range*scaling_factor;
    }
    else
    {
        range=range*1/scaling_factor;
    }

    update();
}


void RotationalLabel::mousePressEvent(QMouseEvent* event){

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
     }

}


void RotationalLabel::mouseMoveEvent(QMouseEvent* event){


    if(event->type() == QEvent::MouseMove)
    {
        if(mousePressed)  mLine.setP2(event->pos());
    }


   if(event->type()==QEvent::MouseMove)
   {
       if(mousePressed) mRect.setBottomRight(event->pos());
   }

    this->update();

}


void RotationalLabel::mouseReleaseEvent(QMouseEvent *event){

  if(event->button()==Qt::LeftButton)  mousePressed = false;
   update();

}


void RotationalLabel::save()
{

    QMessageBox box;
    QAbstractButton* pSaveButton = box.addButton(tr("Save"), QMessageBox::YesRole);

    QPoint side1 = pol.at(1)-pol.at(0);
    QPoint side2 = pol.at(2)-pol.at(1);

    QImage image = QImage(width, height, QImage::Format_RGB16);
    image.fill(Qt::black);

    QRect boundingRect = pol.boundingRect();

    QImage original = pixmap()->copy(boundingRect).toImage();

    QPoint center = QPoint(width/2,height/2);

    int width = sqrt(side1.x()*side1.x()+side1.y()*side1.y());
    int height = sqrt(side2.x()*side2.x()+side2.y()*side2.y());

    int offset_x, offset_y;
    offset_x=(boundingRect.width()-width)/2;
    offset_y =(boundingRect.height()-height)/2;

    double angle=qAtan((float)mLine.dy()/mLine.dx());
    if(angle<0)angle=angle+M_PI;

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
    cords = toCSV(cLine.center().x(), cLine.center().y(),width, height,angle);

    box.setIconPixmap(QPixmap::fromImage(image));


    box.addButton(tr("Don't save"), QMessageBox::NoRole);
    box.exec();

    if (box.clickedButton()==pSaveButton)
    {
       emit imageToBeSavedSignal(image, cords);
    }

    delete pSaveButton;

}


void RotationalLabel::paintEvent(QPaintEvent *event)
{

    QLabel::paintEvent(event);
    painter.begin(this);
    QPen pen(Qt::red, 5);
    painter.setPen(pen);

    if(mousePressed){
        lineSet= true;
        painter.drawLine(mLine);
    }
    else if (lineSet)
    {

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

        painter.drawPolygon(pol);
        cLine = QLine(pol.at(0)+(pol.at(1)-pol.at(0))/2-offset,pol.at(2)+ (pol.at(3)-pol.at(2))/2-offset);

    }
    else if(xSizeSet)
    {

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

        painter.drawPolygon(pol);
       }

}
void RotationalLabel::resetPainting()
{
    painter.end();
}

void RotationalLabel::setMarker(MarkerType marker)
{
    this->marker = marker;
    resetPainting();
}
