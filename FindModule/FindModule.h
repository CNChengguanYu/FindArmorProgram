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
    Mat src_Frame,bin_Frame,dst_bin_Frame,dst_output_Frame,final_Frame;
    COLOR mode = NORMAL;
    vector<Mat> channels;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    vector<Armor> found_Armor;
    vector<LightBar> found_LightBar;
    vector<RotatedRect> found_Rect;

    bool contoursTOLightBar();
    void LightBarTOArmor(vector<LightBar> &input,vector<Armor> &output);
    void Drawrects();
    COLOR GetRectColor(RotatedRect &_Rect);
    bool Armorjudge(LightBar &left_bar , LightBar &right_bar );
    float getLBtoRBlength(LightBar &left_Bar,LightBar &right_Bar);
    bool judgeLightBarAngle(LightBar &left_Bar , LightBar &right_Bar);
    bool judgeLightBarDistance(LightBar &left_Bar , LightBar &right_Bar);
    bool judgeLightBarHeightDiff(LightBar &left_Bar , LightBar &right_Bar);
public:
    bool find(Mat &src);
    void showTargetNumber();
    FindModule(COLOR color);
};


#endif //FINDARMORPROGRAM_FINDMODULE_H
