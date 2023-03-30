#include <iostream>
#include "LightBar/LightBar.h"
#include "FindModule/FindModule.h"
#include "settings.h"
using namespace std;
int main(int args ,char* argv[])
{
    //初始化查找器对象（输入参数暂时没用）
    FindModule Finder(NORMAL);
    //VideoCapture VideoStream(0);
    VideoCapture VideoStream("../armor_red.mp4");
    //初始化帧
    Mat Frame;
    //循环读入帧
    while(VideoStream.read(Frame))
    {
        //开启查找
        if(!Finder.find(Frame))
            std::cout<<"空图"<<std::endl;

        //char comd = waitKey(0);
        //if(comd == 's' or comd == 'S')continue;
        waitKey(10);
    }
    return 0;
}
