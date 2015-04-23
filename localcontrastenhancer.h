#ifndef LOCALCONTRASTENHANCER_H
#define LOCALCONTRASTENHANCER_H

#include <QThread>
#include <opencv2/opencv.hpp>

class LocalContrastEnhancer : public QThread
{
    Q_OBJECT
public:
    enum Mode { Uniform = 1, AHE };
    explicit LocalContrastEnhancer(QObject *parent = 0) {}
    void doWork(cv::Mat &image, float force);
    void setMode(Mode);
protected:
    cv::Mat processAdaptive();
    cv::Mat processUniform();

    float _force;
    cv::Mat _image;
    Mode _mode;
    void run() Q_DECL_OVERRIDE;
signals:
    void resultReady(cv::Mat&);
};

#endif // LOCALCONTRASTENHANCER_H
