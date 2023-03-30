#ifndef FINDARMORPROGRAM_LIGHTBAR_H
#define FINDARMORPROGRAM_LIGHTBAR_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include "../settings.h"
using namespace cv;
class LightBar {
    COLOR light_Color; //灯条颜色
    RotatedRect BarRect; //灯条矩形
    Point2f center;  //灯条中心

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
    COLOR getColor();
    LightBar(COLOR color,RotatedRect Rect);
    LightBar();
    ~LightBar();
};


#endif //FINDARMORPROGRAM_LIGHTBAR_H
