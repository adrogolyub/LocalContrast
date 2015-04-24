#include "localcontrastenhancer.h"
#include <omp.h>
#include <utils.h>
#include <QDebug>

using namespace cv;


void LocalContrastEnhancer::doWork(Mat &image, float force)
{
    _image = image;
    _force = force;    
	TILE_XCOUNT = 2 + 8 * force;
	TILE_YCOUNT = 2 + 8 * force;
    start();
}

void LocalContrastEnhancer::setMode(LocalContrastEnhancer::Mode m)
{
    _mode = m;
}

Mat LocalContrastEnhancer::unsharpMask()
{
    Mat result;
    GaussianBlur(_image, result, cv::Size(0, 0), 3);
    addWeighted(_image, 1.0 + _force * 2, result, -_force * 2, 0, result);
    return result;
}

void LocalContrastEnhancer::run()
{
    Mat result;
    switch (_mode) {
    case AHE:
        result = processAdaptive();
        break;
    case CLAHE:
        result = processAdaptive(true);
        break;
    case UnsharpMask:
        result = unsharpMask();
        break;
    default:
        result = processUniform();
    }    
    emit resultReady(result);
}

Mat LocalContrastEnhancer::processAdaptive(bool limitContrast)
{
	float HIST_LIMIT = 1.0f;
    if (limitContrast) {
        HIST_LIMIT = 0.2 + _force * 0.8;
        TILE_XCOUNT = 6;
        TILE_YCOUNT = 6;
    }
    uchar *map = new uchar[256 * TILE_XCOUNT * TILE_YCOUNT];
    Mat result;
    int w = _image.cols;
    int h = _image.rows;
    // each tile is (2 * dx + 1) * (2 * dy + 1)
    float tileWidth = (w - 1.0f) / (TILE_XCOUNT - 1);
    float tileHeight = (h - 1.0f) / (TILE_YCOUNT - 1);
    int dx = (tileWidth - 1) / 2;
    int dy = (tileHeight - 1) / 2;
    copyMakeBorder(_image, result, dy, dy, dx, dx, BORDER_REFLECT101 | BORDER_ISOLATED);
    int tile = 0; // current tile
    for (float x = dx; x < result.cols - dx; x += tileWidth) {
        for (float y = dy; y < result.rows - dy; y += tileHeight) {
            Rect r(x - dx, y - dy, 2 * dx + 1, 2 * dy + 1);
            Mat m = result(r);
            uchar *pmap = map + tile * 256;
            Utils::createMap(m, pmap, limitContrast, HIST_LIMIT);
            tile++;
        }
    }

    result = _image.clone();
    uchar *data = (uchar*)result.data;
    Mat I;
    cvtColor(_image, I, COLOR_RGB2GRAY);
#pragma omp parallel for
    for (int i = 0; i < w - 1; i++) {
        for (int j = 0; j < h - 1; j++) {
            // tiles indices
            int idx = i / tileWidth;
            int idy = j / tileHeight;
            int ul = idx * TILE_YCOUNT + idy;
            int ur = (idx + 1) * TILE_YCOUNT + idy;
            int bl = idx * TILE_YCOUNT + idy + 1;
            int br = (idx + 1) * TILE_YCOUNT + idy + 1;
            uchar val = I.data[j * w + i];
            int r = 0, g = 0, b = 0;
            if (val > 0) {
                uchar val_ul = map[256 * ul + val];
                uchar val_ur = map[256 * ur + val];
                uchar val_bl = map[256 * bl + val];
                uchar val_br = map[256 * br + val];
                float x1 = idx * tileWidth;
                float y1 = idy * tileHeight;
                float x2 = (idx + 1) * tileWidth;
                float y2 = (idy + 1) * tileHeight;
                float fval = 1.0f / (x2 - x1) / (y2 - y1) *
                        (val_ul * (x2 - i) * (y2 - j) +
                         val_ur * (i - x1) * (y2 - j) +
                         val_bl * (x2 - i) * (j - y1) +
                         val_br * (i - x1) * (j - y1));
                float k = fval / val;
                r = data[3 * j * w + 3 * i] * k;
                g = data[3 * j * w + 3 * i + 1] * k;
                b = data[3 * j * w + 3 * i + 2] * k;
                if (r > 255 || g > 255 || b > 255)
                    r = g = b = 255;
            }
            data[3 * j * w + 3 * i] = r;
            data[3 * j * w + 3 * i + 1] = g;
            data[3 * j * w + 3 * i + 2] = b;
        }
    }
	delete[] map;
    return result;
}

Mat LocalContrastEnhancer::processUniform()
{    
    Mat result = _image.clone();
    uchar map[256];
    Utils::createMap(_image, map);
    Utils::applyMap(map, result);
    return result;
}
