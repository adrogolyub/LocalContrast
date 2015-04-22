#include "utils.h"

using namespace cv;

QPixmap Utils::matToPixmap(Mat &img)
{
    Mat mat = img.clone();
    if (mat.channels() == 1)
        cvtColor(mat, mat, COLOR_GRAY2RGB);
    else if (mat.channels() == 4)
        cvtColor(mat, mat, COLOR_BGRA2RGB);
    else
        cvtColor(mat, mat, COLOR_BGR2RGB);
    return QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
}
