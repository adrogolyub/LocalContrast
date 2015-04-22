#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include "localcontrastenhancer.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
private:
    LocalContrastEnhancer *_enhancer;
    cv::Mat _image;
private slots:
    void onResultReceived(cv::Mat&);
    void updateImage();
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
