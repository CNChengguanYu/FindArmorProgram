#include "LightBar.h"

LightBar::LightBar(COLOR color,RotatedRect Rect) {
    light_Color = color;
    ReRect=Rect;
    center = Rect.center;
    height = Rect.size.height;
    width = Rect.size.width;
    area = Rect.size.area();


    angle=Rect.angle;
    //矫正角度
    if(angle>90)angle=angle-180;
}

LightBar::~LightBar() {

}

LightBar::LightBar() {

}

RotatedRect LightBar::getRect() {
    return ReRect;
}

float LightBar::getHeight() {
    return height;
}

float LightBar::getWidth() {
    return width;
}

Point2f LightBar::getCenter() {
    return ReRect.center;
}

float LightBar::getAngle() {
    return angle;
}


