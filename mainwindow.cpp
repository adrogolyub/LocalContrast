#include "mainwindow.h"
#include "utils.h"
#include <QFileDialog>
#include <QDebug>
#include <QStatusBar>
#include <QButtonGroup>

using namespace std::chrono;

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    modeGroup = new QButtonGroup(this);
    modeGroup->addButton(rbUniform, (int)(LocalContrastEnhancer::Uniform));
    modeGroup->addButton(rbAHE, (int)LocalContrastEnhancer::AHE);
    _enhancer = new LocalContrastEnhancer(this);
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    connect(_enhancer, &LocalContrastEnhancer::resultReady, this, &MainWindow::onResultReceived);
    connect(forceSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(modeGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateImage()));
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image"), QString(), tr("Images (*.png *.bmp *.jpg)"));
    if (fileName.isEmpty())
        return;
    _image = imread(fileName.toLatin1().data(), IMREAD_GRAYSCALE);
    qDebug() << _image.channels();
    imageLeft->setPixmap(Utils::matToPixmap(_image));
    updateImage();
}

void MainWindow::updateImage()
{
    _time = steady_clock::now();
    _enhancer->setMode((LocalContrastEnhancer::Mode)(modeGroup->checkedId()));
    _enhancer->doWork(_image, forceSlider->value() / (float)forceSlider->maximum());
}

void MainWindow::onResultReceived(Mat &res)
{
    long long elapsed =  duration_cast<milliseconds>(steady_clock::now() - _time).count();
    QMainWindow::statusBar()->showMessage(QString("%1 msec").arg(elapsed));
    imageRight->setPixmap(Utils::matToPixmap(res));
}
