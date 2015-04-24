#include "mainwindow.h"
#include "utils.h"
#include <QFileDialog>
#include <QDebug>
#include <QStatusBar>
#include <QButtonGroup>
#include <QMessageBox>

using namespace std::chrono;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    modeGroup = new QButtonGroup(this);
    modeGroup->addButton(rbUniform, (int)(LocalContrastEnhancer::Uniform));
    modeGroup->addButton(rbAHE, (int)LocalContrastEnhancer::AHE);
    modeGroup->addButton(rbCLAHE, (int)LocalContrastEnhancer::CLAHE);
    modeGroup->addButton(rbUS, (int)LocalContrastEnhancer::UnsharpMask);
    _enhancer = new LocalContrastEnhancer(this);
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    connect(_enhancer, &LocalContrastEnhancer::resultReady, this, &MainWindow::onResultReceived);
    connect(forceSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(modeGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateImage()));
    setWindowState(Qt::WindowMaximized);
    forceSlider->hide();
}

void MainWindow::disableGUI(bool t)
{
    forceSlider->setEnabled(!t);
    if (!t)
        forceSlider->setFocus(); // because forceSlider loses focus after disabling
    rbAHE->setEnabled(!t);
    rbCLAHE->setEnabled(!t);
    rbUS->setEnabled(!t);
    rbUniform->setEnabled(!t);
    actionOpen->setEnabled(!t);
    actionSave->setEnabled(!t);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image"), QString(), tr("Images (*.png *.bmp *.jpg)"));
    if (fileName.isEmpty())
        return;
    _image = imread(fileName.toLatin1().data(), IMREAD_COLOR);
    imageLeft->setPixmap(Utils::matToPixmap(_image));
    updateImage();
}

void MainWindow::updateImage()
{
    forceSlider->setVisible(!rbUniform->isChecked());
    disableGUI(); // to avoid synchronization issues
    _time = steady_clock::now();
    _enhancer->setMode((LocalContrastEnhancer::Mode)(modeGroup->checkedId()));
    _enhancer->doWork(_image, forceSlider->value() / (float)forceSlider->maximum());
}

void MainWindow::onResultReceived(Mat &res)
{
    long long elapsed =  duration_cast<milliseconds>(steady_clock::now() - _time).count();
    QMainWindow::statusBar()->showMessage(tr("%1 msec").arg(elapsed));
    lastResult = Utils::matToPixmap(res);
    imageRight->setPixmap(lastResult);
    disableGUI(false);
}

void MainWindow::on_actionSave_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save image"), QString(), tr("PNG Image (*.png)"));
    if (!path.isEmpty()) {
        if (lastResult.save(path))
            QMessageBox::information(this, tr("Image saved"), tr("Image successfully saved."));
        else
            QMessageBox::critical(this, tr("Failure"), tr("Failed to save image."));
    }
}
