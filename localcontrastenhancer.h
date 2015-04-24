#ifndef LOCALCONTRASTENHANCER_H
#define LOCALCONTRASTENHANCER_H

#include <QThread>
#include <opencv2/opencv.hpp>
#include <QMutex>

class LocalContrastEnhancer : public QThread
{
    Q_OBJECT
public:
    enum Mode { Uniform = 1, AHE, CLAHE, UnsharpMask };
    explicit LocalContrastEnhancer(QObject *) {}
    void doWork(cv::Mat &image, float force);
    void setMode(Mode);
protected:
	uchar *map;
    QMutex _mutex;
    cv::Mat unsharpMask();
    cv::Mat processAdaptive(bool limitContrast = false);
    cv::Mat processUniform();
	int TILE_XCOUNT;
	int TILE_YCOUNT;
    float _force;
    cv::Mat _image;
    Mode _mode;
    void run() Q_DECL_OVERRIDE;
signals:
    void resultReady(cv::Mat&);
};

#endif // LOCALCONTRASTENHANCER_H
