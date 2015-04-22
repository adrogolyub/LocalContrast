#include "mainwindow.h"
#include "utils.h"
#include <QFileDialog>
#include <QDebug>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    _enhancer = new LocalContrastEnhancer(this);
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    connect(_enhancer, &LocalContrastEnhancer::resultReady, this, &MainWindow::onResultReceived);
    connect(forceSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image"), QString(), tr("Images (*.png *.bmp *.jpg)"));
    if (fileName.isEmpty())
        return;
    _image = imread(fileName.toLatin1().data(), IMREAD_UNCHANGED);
    qDebug() << _image.channels();
    imageLeft->setPixmap(Utils::matToPixmap(_image));
}

void MainWindow::updateImage()
{
    _enhancer->doWork(_image, forceSlider->value() / (float)forceSlider->maximum());
}

void MainWindow::onResultReceived(Mat &res)
{
    imageRight->setPixmap(Utils::matToPixmap(res));
}
