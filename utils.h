#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <QPixmap>

class Utils
{
public:
    static QPixmap matToPixmap(const cv::Mat &img);
    static void applyMap(uchar *map, cv::Mat &m);

    // create histogram equalization mapping for image intensity
    static void createMap(const cv::Mat &m, uchar *map,
                          bool limitContrast = false,
                          float HIST_LIMIT = 1.0f);
};

#endif // UTILS_H
