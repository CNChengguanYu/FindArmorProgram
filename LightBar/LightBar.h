#ifndef FINDARMORPROGRAM_LIGHTBAR_H
#define FINDARMORPROGRAM_LIGHTBAR_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include "../settings.h"
using namespace cv;
class LightBar {
    COLOR light_Color;
    RotatedRect ReRect;
    Point2f center;

    float angle=0;
    float area=0;
    float height=0;
    float width=0;

public:
    RotatedRect getRect();
    float getHeight();
    float getWidth();
    float getAngle();
    Point2f getCenter();
    LightBar(COLOR color,RotatedRect Rect);
    LightBar();
    ~LightBar();
};


#endif //FINDARMORPROGRAM_LIGHTBAR_H
