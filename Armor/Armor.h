
#ifndef FINDARMORPROGRAM_ARMOR_H
#define FINDARMORPROGRAM_ARMOR_H
#include "../LightBar/LightBar.h"
#include<cmath>
class Armor {
    LightBar l_LightBar,r_LightBar;
    Point2f points[4];
    Point2f box_Center;
    ID box_ID = UNKNOWN;
    float LB_to_RB_Lenght=0;
    float angle=0;
    void AVGangle();
    void BuildArmorBox();
public:
    Armor();
    Armor(LightBar &_l_bar,LightBar &_r_bar);
    Mat DisplayBox(const Mat& Frame);
    Point2f getBoxCenter();
    float getLBtoRBLenght();
    void GetCenterRectPoints(Point2f *ptr);
    LightBar getLeftLightBar();
    LightBar getRightLightBar();
};


#endif //FINDARMORPROGRAM_ARMOR_H
