#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
private:
    cv::Mat _image;
private slots:
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
