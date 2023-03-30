
#ifndef FINDARMORPROGRAM_SETTINGS_H
#define FINDARMORPROGRAM_SETTINGS_H
enum COLOR
{
    RED=0,
    BLUE=1,
    NORMAL=2
};
enum ID   //这个暂时没用
{
    UNKNOWN=0,
    RED_HERO=1,
    RED_ENGINEER=2,
    RED_INFANTRY_3=3,
    RED_INFANTRY_4=4,
    RED_INFANTRY_5=5,
    BLUE_HERO=6,
    BLUE_ENGINEER=7,
    BLUE_INFANTRY_3=8,
    BLUE_INFANTRY_4=9,
    BLUE_INFANTRY_5=10,
};
//允许的最大角度差
#define MAX_ANGLE_DIFF 10
//允许灯条的最大角度
#define MAX_LIGHTBAR_ANGLE 50
//允许灯条的最小面积
#define MIN_LIGHTBAR_AREA 200
//允许灯条的最大面积
#define MAX_LIGHTBAR_AREA 16000
//最大灯条高度差系数 （差不能大于自身*系数）
#define MAX_LIGHTBAR_HEIGHT_DIFF_COEF 1.1
//最大灯条高度差系数（差不能小于自身*系数）
#define MIN_LIGHTBAR_HEIGHT_DIFF_COEF 0.9
//三角形角度的最大值
#define MAX_COS_VALUE 0.8
//矩形最大距离差系数（差不能大于自身*系数）
#define MAX_DISTANCE_COEF 1.5
//矩形最小距离差系数（差不能小于自身*系数）
#define MIN_DISTANCE_COEF 0.7
//装甲板数字标签输入大小(不建议设太大)
#define ORGIN_TARGET_AREA 256
//装甲板数字标签压缩后大小(不建议设太大)
#define DST_TARGET_AREA 28
#endif //FINDARMORPROGRAM_SETTINGS_H
