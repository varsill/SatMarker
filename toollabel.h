#ifndef TOOLLABEL_H
#define TOOLLABEL_H
#include <QLabel>



class ToolLabel : public QLabel
{

QPainter painter;


signals:

    void imageToBeSavedSignal(QImage, QString);


public:

    ToolLabel();
    virtual ~ToolLabel();


private:

    virtual void saveImage()=0;
    virtual void resetPainting() = 0;
    virtual void mousePressEvent(QMouseEvent*)=0;
    virtual void mouseMoveEvent(QMouseEvent*)=0;
    virtual void mouseReleaseEvent(QMouseEvent*)=0;
    virtual void paintEvent(QPaintEvent*)=0;
    virtual void wheelEvent(QWheelEvent*)=0;
    virtual void keyPressEvent(QKeyEvent*)=0;


protected:

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

};

#endif // TOOLLABEL_H
