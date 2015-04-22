#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <QPixmap>

class Utils
{
public:
    static QPixmap matToPixmap(cv::Mat &img);
};

#endif // UTILS_H
