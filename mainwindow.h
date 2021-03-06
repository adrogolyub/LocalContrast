#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include "localcontrastenhancer.h"
#include <QButtonGroup>
#include <chrono>
#include <vector>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
private:    
    std::vector<cv::Mat> _splitLabImage;
    cv::Mat _image;
    std::chrono::steady_clock::time_point _time;
    LocalContrastEnhancer *_enhancer;
    //cv::Mat _image;
    QButtonGroup *modeGroup;
    QPixmap lastResult;
    void disableGUI(bool t = true);
private slots:
    void onResultReceived(cv::Mat&);
    void updateImage();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
};

#endif // MAINWINDOW_H
