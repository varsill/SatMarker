#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QScrollArea>
#include <QStringList>
#include <QPushButton>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <iostream>
#include "extrascrollarea.h"
#include "rotationalToolLabel.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void nextButtonClicked();
    void zoomIn();
    void zoomOut();
    void changeInputDir();
    void changeOutputDir();
    void aCiulNieMa();
    void setRectangleMarker();
    void setRotationalMarker();
private:
    void loadImage(QString);
    void createMenus();
    void loadNextImage();
    void printImagesPaths();
    void scale(double);
    void getFilesList();
    void saveImage(QImage, QString);
    void saveCords(QString);
    QString getInputDir();
    QString getOutputDir();
    double scaleFactor;
    QPixmap pixmap;
    RotationalLabel* label;
    QString settingsFile;
    QString inputDirectoryPath;
    QString outputDirectoryPath;
    QPushButton* nextButton;
    QWidget* wid;
    RotationalScrollArea* scrollArea;
    QVBoxLayout *layout;
    QStringList * filesList;
    MarkerType marker;
    QMenu* directoryMenu;
    QMenu* viewMenu;
    QMenu* toolsMenu;

    QAction* inputDirAction;
    QAction* outputDirAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* setRotationalMarkerAction;
    QAction* setRectangleMarkerAction;
};

#endif // MAINWINDOW_H
