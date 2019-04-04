#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
      settingsFile=QDir::currentPath() + "/settings.ini";
      scaleFactor=1.0;
      scrollArea = new ExtraScrollArea();
      label = new ExtraLabel();
      layout= new QVBoxLayout;
      wid = new QWidget;
      nextButton = new QPushButton("Next Image");
      filesList = new QStringList;

      //Actions:
      inputDirAction=new QAction("Input Directory Settings");
      outputDirAction=new QAction("Output Directory Settings");
      zoomInAction = new QAction("Zoom in");
      zoomOutAction = new QAction("Zoom out");
      setRectangleMarkerAction = new QAction("Set rectangle marker");
      setRotationalMarkerAction = new QAction("Set rotational marker");


      //Set Layout:
      layout->addWidget(scrollArea);
      layout->addWidget(nextButton);
      layout->setMargin(5);
      setCentralWidget(wid);
      wid->setLayout(layout);
      scrollArea->setWidget(label);

      createMenus();
      inputDirectoryPath=getInputDir();

       getFilesList();
       loadNextImage();


       connect(nextButton, SIGNAL(clicked(bool)), this, SLOT(nextButtonClicked()));
       connect(label, &ExtraLabel::imageToBeSaved, this, &MainWindow::saveImage);

       connect(inputDirAction, SIGNAL(triggered(bool)), this, SLOT(changeInputDir()));
       connect(outputDirAction, SIGNAL(triggered(bool)), this, SLOT(changeOutputDir()));

       connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(aCiulNieMa()));
       connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(aCiulNieMa()));

       connect(setRotationalMarkerAction, SIGNAL(triggered(bool)), this, SLOT(setRotationalMarker()));
       connect(setRectangleMarkerAction, SIGNAL(triggered(bool)), this, SLOT(setRectangleMarker()));

}



MainWindow::~MainWindow()
{
    delete filesList;
    delete scrollArea;
    delete layout;
    delete directoryMenu;
    delete nextButton;
    delete label;
    delete wid;
    delete inputDirAction;
    delete outputDirAction;
    delete zoomInAction;
    delete zoomOutAction;
    delete viewMenu;



}


void MainWindow::saveImage(QImage image, QString cords)
{
    QString dirPath = getOutputDir();
    if(dirPath.isEmpty()) changeOutputDir();

    QDir directory(dirPath);
    QStringList list= directory.entryList(QStringList() <<  "*.png",QDir::Files);
    QRegExp regex("^[0-9]{1,9}\.png$");

    list = list.filter(regex);

    int maxi = 0;
    for(auto i = list.begin(); i!=list.end(); i++)
    {
        QString num = *i;
        num = num.left(num.indexOf("."));
        if(num.toInt()>maxi)maxi=num.toInt();
    }

    QString path = dirPath+QString::number(maxi+1)+".png";
    image.save(path);
    saveCords(cords);
}

void MainWindow::saveCords(QString cords)
{
    QString filename=getOutputDir()+"cords.csv";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
    {
        QTextStream stream( &file );
        stream << cords << endl;
    }
}


//Handling Actions:


void MainWindow::changeInputDir()
{

   QSettings settings(settingsFile, QSettings::IniFormat);
   QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                       "/home",
                                                                       QFileDialog::ShowDirsOnly
                                                                       | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty()) settings.setValue("input_dir", dir+"/");
    getFilesList();
    loadNextImage();

}


void MainWindow::getFilesList()
{
    QDir directory(getInputDir());
    *filesList =  directory.entryList(QStringList() << "*.bmp" << "*.png",QDir::Files);

}


void MainWindow::changeOutputDir()
{
    QSettings settings(settingsFile, QSettings::IniFormat);

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                       "/home",
                                                                       QFileDialog::ShowDirsOnly
                                                                       | QFileDialog::DontResolveSymlinks);

   if(!dir.isEmpty())  settings.setValue("output_dir", dir+"/");

}


QString MainWindow::getInputDir()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    QString result =settings.value("input_dir", "").toString();
    return result;
}

QString MainWindow::getOutputDir()
{
 QSettings settings(settingsFile, QSettings::IniFormat);
 QString result =settings.value("output_dir", "").toString();
 return result;
}


void MainWindow::loadImage(QString path)
{
    pixmap = QPixmap(path);
    label->setPixmap(pixmap);
    label->adjustSize();

    label->update();


}


void MainWindow::nextButtonClicked()
{
    loadNextImage();
}


void MainWindow::printImagesPaths()
{

    foreach (const QString path, *filesList) {
       cout<<path.toStdString()<<endl;
    }
}

void MainWindow::loadNextImage()
{

        if(!filesList->isEmpty())
        {
            QString path = getInputDir()+filesList->at(0);
            loadImage(path);
            filesList->pop_front();
        }


}


void MainWindow::createMenus()
{
   directoryMenu = menuBar()->addMenu(tr("&Directory settings"));
   directoryMenu->addAction(inputDirAction);
   directoryMenu->addAction(outputDirAction);


   viewMenu=menuBar()->addMenu(tr("&View"));
   viewMenu->addAction(zoomInAction);
   viewMenu->addAction(zoomOutAction);

   toolsMenu = menuBar()->addMenu(tr("&Tools"));
   toolsMenu->addAction(setRectangleMarkerAction);
   toolsMenu->addAction(setRotationalMarkerAction);


}


//Scaling Functions:


void adjustScrollBar(QScrollBar* scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                             + ((factor - 1) * scrollBar->pageStep()/2)));

}


void MainWindow::scale(double factor)
{
    scaleFactor*=factor;
    if(scaleFactor<0.25 || scaleFactor>4) return;
   // label->resize(label->pixmap()->size()*scaleFactor);
//    label->setPixmap(pixmap.scaled(pixmap.width()*scaleFactor, pixmap.height()*scaleFactor, Qt::KeepAspectRatio));
    label->resize(scaleFactor * pixmap.size());
    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
}


void MainWindow::zoomIn()
{
  //  scale(1.25);
}


void MainWindow::zoomOut()
{
 //   scale(0.75);
}


void MainWindow::aCiulNieMa()
{
     QMessageBox box;
     box.setText("A ciul, nie ma zooma, bo mi sie zaimplementowac nie chcialo xDDD");
     box.exec();
}


void MainWindow::setRotationalMarker()
{
    this->label->setMarker(RotationalMarker);
}

void MainWindow::setRectangleMarker()
{
    this->label->setMarker(RectangleMarker);
}
