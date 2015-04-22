#include "localcontrastenhancer.h"

void LocalContrastEnhancer::doWork(cv::Mat &image, float force)
{
    _image = image;
    _force = force;
    start();
}

void LocalContrastEnhancer::run()
{
    emit resultReady(_image);
}
