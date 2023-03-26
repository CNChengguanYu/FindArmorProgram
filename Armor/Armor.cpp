#include "Armor.h"

Armor::Armor(LightBar &_l_bar,LightBar &_r_bar) {
    l_LightBar=_l_bar;
    r_LightBar=_r_bar;
    //获取灯条平均角度
    AVGangle();
    //构造装甲版中心
    BuildArmorBox();
    //获取灯条间距
    LB_to_RB_Lenght=getLBtoRBLenght();
    //获取装甲版中心
    box_Center=getBoxCenter();
}

Armor::Armor() {

}

void Armor::GetCenterRectPoints(Point2f *ptr) {
    //返回装甲版四边坐标
    ptr[0]=points[0];
    ptr[1]=points[1];
    ptr[2]=points[2];
    ptr[3]=points[3];
}

void Armor::AVGangle() {
    angle= (l_LightBar.getAngle() + r_LightBar.getAngle()) / 2;
    //std::cout << "133312angle:" << l_LightBar.getAngle() << "," << r_LightBar.getAngle() << std::endl;
}

void Armor::BuildArmorBox() {
    //构造 装甲版左-右矩形（大小），将灯条高X2后得出高度
    Size l_LightBar_rect(l_LightBar.getWidth()+4, l_LightBar.getHeight() * 2);
    Size r_LightBar_rect(r_LightBar.getWidth()+4, r_LightBar.getHeight() * 2);
    //矩形的顶点坐标
    Point2f l_LightBar_rect_points[4]={};
    Point2f r_LightBar_rect_points[4]={};
    //构造左-右矩形
    RotatedRect l_Rect(l_LightBar.getCenter(), l_LightBar_rect, l_LightBar.getAngle());
    RotatedRect r_Rect(r_LightBar.getCenter(), r_LightBar_rect, r_LightBar.getAngle());
    //获取左-右矩形的顶点
    l_Rect.points(l_LightBar_rect_points);
    r_Rect.points(r_LightBar_rect_points);

    //装甲版矩形的左下->左上->右上->右下的坐标分别为
    //左矩形的[2]/[3]
    //右矩形的[0]/[1]
    points[0]=l_LightBar_rect_points[3];
    points[1]=l_LightBar_rect_points[2];
    points[2]=r_LightBar_rect_points[1];
    points[3]=r_LightBar_rect_points[0];

}

float Armor::getLBtoRBLenght() {
    float width =abs(r_LightBar.getCenter().x-l_LightBar.getCenter().x);
    float height =abs(l_LightBar.getCenter().y-r_LightBar.getCenter().y);
    return sqrt(width*width+height*height);
}

Point2f Armor::getBoxCenter() {
    float x,y;
    float width=r_LightBar.getCenter().x-l_LightBar.getCenter().x
          ,height=abs(r_LightBar.getCenter().y-l_LightBar.getCenter().y);
    x = l_LightBar.getCenter().x;
    y = MAX(l_LightBar.getCenter().y,r_LightBar.getCenter().y);
    Point2f center(x+(width/2)
                   ,y-(height/2));
    return center;
}

Mat Armor::DisplayBox(const Mat& Frame) {
    Point2f apex[4];
    apex[0]=Point2f (0,256);
    apex[1]=Point2f (0,0);
    apex[2]=Point2f (256,0);
    apex[3]=Point2f (256,256);


    Mat out_Display = Mat::zeros(Size(256,256),CV_8UC3);
    Mat warp_Matrix = getPerspectiveTransform(points,apex);
    warpPerspective(Frame, out_Display, warp_Matrix, out_Display.size(), INTER_CUBIC);
//    cvtColor(out_Display,out_Display,COLOR_BGR2GRAY);
//    threshold(out_Display,out_Display,10,255,THRESH_BINARY);
//    dilate(out_Display,out_Display, getStructuringElement(MORPH_RECT,Size(11,11)));
//    erode(out_Display,out_Display,getStructuringElement(MORPH_RECT, Size(7, 7)));
    resize(out_Display,out_Display,Size(28,28));
    return out_Display;
}

LightBar Armor::getLeftLightBar() {
    return l_LightBar;
}

LightBar Armor::getRightLightBar() {
    return r_LightBar;
}
