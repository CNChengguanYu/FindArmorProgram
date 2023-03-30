#include "Armor.h"

Armor::Armor(LightBar &_l_bar,LightBar &_r_bar) {
    /*构造装甲板*/

    //初始化灯条
    l_LightBar=_l_bar;
    r_LightBar=_r_bar;
    //获取颜色（左右随机取一个）
    color=_l_bar.getColor();
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
        std::cout<<"NULL!"<<std::endl;
}

void Armor::getArmorBoxPoints(Point2f *ptr) {
    /*返回装甲版四边坐标*/

    ptr[0]=points[0];
    ptr[1]=points[1];
    ptr[2]=points[2];
    ptr[3]=points[3];
}

void Armor::AVGangle() {
    /*获取平均角度*/
    angle= (l_LightBar.getAngle() + r_LightBar.getAngle()) / 2;
}

void Armor::BuildArmorBox() {
    /*构造 装甲版左-右矩形（大小）*/

    //将灯条宽度+4，目的是扩大灯条面积，减少装甲板中心面积，避免中心受到灯光影响，将灯条高X2后得出高度
    Size l_LightBar_rect(l_LightBar.getWidth()+4, l_LightBar.getHeight() * 2);
    Size r_LightBar_rect(r_LightBar.getWidth()+4, r_LightBar.getHeight() * 2);
    //定义矩形的顶点坐标
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
    /*获取左-右灯条长度（根据勾股定理）*/

    float width =abs(r_LightBar.getCenter().x-l_LightBar.getCenter().x);
    float height =abs(l_LightBar.getCenter().y-r_LightBar.getCenter().y);
    //长^2+宽^2 开根号
    return sqrt(width*width+height*height);
}

Point2f Armor::getBoxCenter() {
    /*获取装甲版中心*/

    float x,y;
    //获取矩形长、宽(以灯条中心为端点，拟合一个平行于X-Y轴的矩形 ，矩形的对角线中心必为装甲板中心)
    float width=r_LightBar.getCenter().x-l_LightBar.getCenter().x
          ,height=abs(r_LightBar.getCenter().y-l_LightBar.getCenter().y);
    //以x,y为原点
    x = l_LightBar.getCenter().x;
    y = MAX(l_LightBar.getCenter().y,r_LightBar.getCenter().y);

    //OPENCV中，y坐标轴向下为正，故应该减去高度（在图像中向上）
    Point2f center(x+(width/2)
                   ,y-(height/2));
    return center;
}

Mat Armor::DisplayBox(const Mat& Frame) {
    /*显示装甲板数字*///按能力理解即可，不重要

    //创建顶点
    Point2f apex[4];
    //设置为矩形的四角顶点（宽度ORGIN_TARGET_AREA）
    apex[0]=Point2f (0,ORGIN_TARGET_AREA);
    apex[1]=Point2f (0,0);
    apex[2]=Point2f (ORGIN_TARGET_AREA,0);
    apex[3]=Point2f (ORGIN_TARGET_AREA,ORGIN_TARGET_AREA);

    //输出矩形
    Mat out_Display = Mat::zeros(Size(ORGIN_TARGET_AREA,ORGIN_TARGET_AREA),CV_8UC3);
    //透射矩阵（详情搜『透射变换』）
    Mat warp_Matrix = getPerspectiveTransform(points,apex);
    //应用透射变换，纠正形变
    warpPerspective(Frame, out_Display, warp_Matrix, out_Display.size(), INTER_CUBIC);

    //cvtColor(out_Display,out_Display,COLOR_BGR2GRAY);
    //threshold(out_Display,out_Display,5,255,THRESH_BINARY);

    //缩小输出面积，减少SVM计算量
    resize(out_Display,out_Display,Size(DST_TARGET_AREA,DST_TARGET_AREA));
    return out_Display;
}

LightBar Armor::getLeftLightBar() {
    return l_LightBar;
}

LightBar Armor::getRightLightBar() {
    return r_LightBar;
}

COLOR Armor::getColor() {
    return color;
}
