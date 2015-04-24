#include "utils.h"
#include <QDebug>

using namespace cv;

QPixmap Utils::matToPixmap(const Mat &img)
{
    Mat mat;
    if (img.channels() == 1)
        cvtColor(img, mat, COLOR_GRAY2RGBA);
    else if (img.channels() == 4)
        cvtColor(img, mat, COLOR_BGRA2RGBA);
    else
        cvtColor(img, mat, COLOR_BGR2RGBA);
    QPixmap res = QPixmap::fromImage(QImage(mat.data, mat.cols, mat.rows, QImage::Format_ARGB32));
    return res;
}

void Utils::createMap(const Mat &_m, uchar *map, bool limitContrast, float HIST_LIMIT)
{
    Mat I;
    if (_m.channels() == 3)
        cvtColor(_m, I, COLOR_RGB2GRAY);
    else
        I = _m;
    int hist[256];
    int cdf[256];
    memset(hist, 0, 256 * sizeof(int));
    memset(cdf, 0, 256 * sizeof(int));
    uchar *data = I.data;
    int w = _m.cols, h = _m.rows;
    int N = w * h;
    for (int i = 0; i < N; i++)
        hist[data[i]]++;
    if (limitContrast) {
        int excess = 0;
        int maxCount = 0;
        for (int i = 0; i < 256; i++)
            maxCount = MAX(maxCount, hist[i]);
        int limit = HIST_LIMIT * maxCount;
        for (int i = 0; i < 256; i++) {
            if (hist[i] > limit) {
                excess += hist[i] - limit;
                hist[i] = limit;
            }
        }
        for (int i = 0; i < 256; i++) {
            hist[i] += excess / 256;
        }
    }
    int minColor = -1;
    int maxColor = -1;
    for (int i = 0; i < 256; i++)
        if (hist[i] > 0) {
            minColor = i;
            break;
        }
    for (int i = 255; i >=0; i--)
        if (hist[i] > 0) {
            maxColor = i;
            break;
        }
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++)
        cdf[i] = cdf[i-1] + hist[i];
    for (int i = 0; i < 256; i++)
        map[i] = (uchar)(float(cdf[i] - cdf[minColor]) / (N - cdf[minColor]) * 255 + 0.5f);
}

void Utils::applyMap(uchar *map, Mat &m)
{
    int N = m.cols * m.rows;
    uchar *data = m.data;
    if (m.channels() == 3) {
        Mat I;
        cvtColor(m, I, COLOR_RGB2GRAY);

        for (int i = 0; i < N; i++) {
            int r = 0, g = 0, b = 0;
            if (I.data[i] > 0) {
                float k = float(map[I.data[i]]) / I.data[i];
                r = data[3 * i] * k;
                g = data[3 * i + 1] * k;
                b = data[3 * i + 2] * k;
                if (r > 255 || g > 255 || b > 255)
                    r = g = b = 255;
            }
            data[3 * i] = r;
            data[3 * i + 1] = g;
            data[3 * i + 2] = b;
        }
    }
    else {
        for (int i = 0; i < N; i++)
            data[i] = map[data[i]];
    }
}

/*
void testHE()
{
    unsigned char a[] = {52, 55, 61,  66,  70,  61, 64, 73,
                         63, 59, 55,  90, 109,  85, 69, 72,
                         62, 59, 68, 113, 144, 104, 66, 73,
                         63, 58, 71, 122, 154, 106, 70, 69,
                         67, 61, 68, 104, 126,  88, 68, 70,
                         79, 65, 60,  70,  77,  68, 58, 75,
                         85, 71, 64,  59,  55,  61, 65, 83,
                         87, 79, 69,  68,  65,  76, 78, 94};
    Mat m(8, 8, CV_8UC1, a);
    uchar map[256];
    calcMap(m, map);
    useMap(map, m);
}*/
