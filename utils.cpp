#include "utils.h"
#include <QDebug>

using namespace cv;

QPixmap Utils::matToPixmap(Mat &img)
{
    Mat mat;
    if (img.channels() == 1)
        cvtColor(img, mat, COLOR_GRAY2RGBA);
    else if (img.channels() == 4)
        cvtColor(img, mat, COLOR_BGRA2RGBA);
    else
        cvtColor(img, mat, COLOR_BGR2RGBA);
    return QPixmap::fromImage(QImage(mat.data, mat.cols, mat.rows, QImage::Format_ARGB32));
}
