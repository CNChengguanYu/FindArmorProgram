
#ifndef FINDARMORPROGRAM_ARMOR_H
#define FINDARMORPROGRAM_ARMOR_H
#include "../LightBar/LightBar.h"
#include<cmath>
class Armor {
    LightBar l_LightBar,r_LightBar;  //左右灯条
    Point2f points[4];  //装甲板顶点
    Point2f box_Center; //装甲板中心点
    ID box_ID = UNKNOWN;    //装甲板ID
    COLOR color;        //装甲板颜色
    float LB_to_RB_Lenght=0;  //灯条间距
    float angle=0; //装甲板角度
    void AVGangle();    //获取平均角度
    void BuildArmorBox();   //构造装甲板中心
public:
    Armor();//构造函数
    Armor(LightBar &_l_bar,LightBar &_r_bar);//构造函数
    Mat DisplayBox(const Mat& Frame);//显示装甲板数字
    Point2f getBoxCenter();//获取装甲板中心
    float getLBtoRBLenght();//获取间距
    void getArmorBoxPoints(Point2f *ptr);//获取装甲板顶点
    COLOR getColor();//获取颜色
    LightBar getLeftLightBar();//获取灯条
    LightBar getRightLightBar();//获取灯条
};
#endif //FINDARMORPROGRAM_ARMOR_H
