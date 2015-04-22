#ifndef LOCALCONTRASTENHANCER_H
#define LOCALCONTRASTENHANCER_H

#include <QThread>
#include <opencv2/opencv.hpp>

class LocalContrastEnhancer : public QThread
{
    Q_OBJECT
    float _force;
    cv::Mat _image;
    void run() Q_DECL_OVERRIDE;
public:
    explicit LocalContrastEnhancer(QObject *parent = 0) {}
    void doWork(cv::Mat &image, float force);
signals:
    void resultReady(cv::Mat);
};

#endif // LOCALCONTRASTENHANCER_H
