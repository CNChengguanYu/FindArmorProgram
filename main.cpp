#include <iostream>
#include "LightBar/LightBar.h"
#include "FindModule/FindModule.h"
#include "settings.h"
using namespace std;
int main(int args ,char* argv[])
{
    FindModule Finder(BLUE);
    //VideoCapture VideoStream(0);
    VideoCapture VideoStream("../armor_red.mp4");
    Mat Frame;
    while(VideoStream.read(Frame))
    {
        if(!Finder.find(Frame))
            std::cout<<"空图"<<std::endl;

        //char comd = waitKey(0);
        //if(comd == 's' or comd == 'S')continue;
        waitKey(10);
    }
    return 0;
}
