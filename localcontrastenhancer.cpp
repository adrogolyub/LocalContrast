#include "localcontrastenhancer.h"
#include <omp.h>
using namespace cv;

#define TILE_XCOUNT 8
#define TILE_YCOUNT 8

void LocalContrastEnhancer::doWork(cv::Mat &image, float force)
{
    _image = image;
    _force = force;    
    start();
}

void LocalContrastEnhancer::setMode(LocalContrastEnhancer::Mode m)
{
    _mode = m;
}

void LocalContrastEnhancer::run()
{
    Mat result;
    switch (_mode) {
    case AHE:
        result = processAdaptive();
        break;
    default:
        result = processUniform();
    }
    emit resultReady(result);
}

void calcMap(const Mat &m, uchar *map)
{
    int hist[256];
    int cdf[256];
    memset(hist, 0, 256 * sizeof(int));
    memset(cdf, 0, 256 * sizeof(int));
    uchar *data = m.data;
    int w = m.cols, h = m.rows;
    int N = w * h;
    for (int i = 0; i < N; i++)
        hist[data[i]]++;
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

Mat LocalContrastEnhancer::processAdaptive()
{
    uchar map[256 * TILE_XCOUNT * TILE_YCOUNT];

    // each tile is (2 * dx + 1) * (2 * dy + 1)
    int dx = (_image.cols / TILE_XCOUNT - 1) / 2;
    int dy = (_image.rows / TILE_YCOUNT - 1) / 2;
    int tile = 0; // current tile
    for (int x = dx + 1; x < _image.cols; x += 2 * dx) {
        for (int y = dy + 1; y < _image.rows; y += 2 * dy) {

        }
    }
    Mat result = _image.clone();
    uchar *data = result.data;
    return result;
}

Mat LocalContrastEnhancer::processUniform()
{    
    Mat result = _image.clone();
    uchar *data = result.data;    
    int N = result.cols * result.rows;
    uchar map[256];

    calcMap(_image, map);
//#pragma omp parallel for
    for (int i = 0; i < N; i++)
        data[i] = map[data[i]];
    return result;
}
