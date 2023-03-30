//
// Created by master on 23-3-9.
//

#ifndef FINDARMORPROGRAM_FINDMODULE_H
#define FINDARMORPROGRAM_FINDMODULE_H
#include<opencv2/opencv.hpp>
#include "../settings.h"
#include <vector>
#include "../Armor/Armor.h"

using namespace cv;
using namespace std;
class FindModule {
    //原始帧，二值帧、二值处理帧、最终帧，ROI
    Mat src_Frame,bin_Frame,dst_bin_Frame,final_Frame,ROI;
    //颜色强度量
    int red_color=0;
    int blue_color=0;
    COLOR mode = NORMAL;
    vector<vector<Point>> contours;//找到的轮廓
    vector<Vec4i> hierarchy;//拓扑关系（没用到）
    vector<Armor> found_Armor;//查找到的装甲板
    vector<LightBar> found_LightBar;//查找到的灯条
    vector<RotatedRect> found_Rect;//查找到的矩形


    COLOR GetRectColor(RotatedRect &_Rect);//获取颜色
    bool contoursTOLightBar();//轮廓->灯条
    void LightBarTOArmor(vector<LightBar> &input,vector<Armor> &output);//灯条->装甲板
    void Drawrects();//绘制图像
    bool Armorjudge(LightBar &left_bar , LightBar &right_bar );//条件判断
    float getLBtoRBlength(LightBar &left_Bar,LightBar &right_Bar);//获取距离
    bool judgeLightBarAngle(LightBar &left_Bar , LightBar &right_Bar);//角度差
    bool judgeLightBarDistance(LightBar &left_Bar , LightBar &right_Bar);//距离判断
    bool judgeLightBarHeightDiff(LightBar &left_Bar , LightBar &right_Bar);//三角形角度-高 差
    bool judgeLightHeight(LightBar &left_Bar , LightBar &right_Bar);//高度差
public:
    bool find(Mat &src);//查找函数
    void showTargetNumber();//开启标签显示
    FindModule(COLOR color);//构造函数
    FindModule();
    ~FindModule();
};


#endif //FINDARMORPROGRAM_FINDMODULE_H
