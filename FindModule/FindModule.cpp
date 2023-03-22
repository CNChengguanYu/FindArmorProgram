//
// Created by master on 23-3-9.
//

#include "FindModule.h"
bool FindModule::find(Mat &src) {
    if(src.empty()){
        return false;
    }
    src_Frame=src;
    found_Armor.clear();
    src.copyTo(final_Frame);

    split(src,channels);

    cvtColor(src,bin_Frame,COLOR_BGR2GRAY);

    //imshow("321",bin_Frame);

    threshold(bin_Frame,dst_bin_Frame,230,255,THRESH_BINARY);

    //开闭
    dilate(dst_bin_Frame,dst_bin_Frame, getStructuringElement(MORPH_RECT,Size(11,11)));
    erode(dst_bin_Frame,dst_bin_Frame,getStructuringElement(MORPH_RECT, Size(7, 7)));
    //dilate(dst_bin_Frame,dst_bin_Frame, getStructuringElement(MORPH_RECT,Size(3,3)));

    //imshow("dst_bin",dst_bin_Frame);


    findContours(dst_bin_Frame,contours,hierarchy,RETR_CCOMP, CHAIN_APPROX_NONE);

    if(contoursTOLightBar())//cout<<"找到灯条"<<found_LightBar.size()<<endl;

    LightBarTOArmor(found_LightBar,found_Armor);

    Drawrects();

    //cout<<"装甲版"<<found_Armor.size()<<"个"<<endl;


    showTargetNumber();

    imshow("test",final_Frame);
    return true;
}

FindModule::FindModule(COLOR color) {
    mode = color ;
}

bool FindModule::contoursTOLightBar() {
    //清空上一帧的灯条
    found_LightBar.resize(0);
    //根据画面边缘构造灯条
    for(vector<Point> index_contours:contours)
    {
       if(index_contours.size()<6)continue;
       RotatedRect get_Rect;
       //从椭圆构造矩形,在方向上比从边缘构建更准确
       get_Rect = fitEllipse(index_contours);
       //如果面积小于100，则剔除（减少多余运算）
       if(get_Rect.size.area()<100)continue;
       //构造灯条
       found_LightBar.push_back(LightBar(GetRectColor(get_Rect),get_Rect));
    }
    //从左到右开始排序,x值小的排到末尾
    sort(found_LightBar.begin(), found_LightBar.end(),
         [](LightBar & L1, LightBar & L2) {
             return L1.getCenter().x > L2.getCenter().x; });
    //如果构造到的灯条大于2，则返回true
    if(found_LightBar.size()>2)return true;
    return false;
}

void FindModule::Drawrects() {
    //需要绘制的四个顶点
    Point2f rect_Points[4]={};
    //绘制灯条
//    for(LightBar index :found_LightBar)
//    {
//        index.getRect().points(rect_Points);
//        putText(final_Frame,"Angle:"+to_string((int)index.getAngle()),index.getCenter(),1,2,Scalar(255,0,0),2);
//        for(int draw_num=0;draw_num<4;++draw_num)
//        {
//            line(final_Frame,rect_Points[draw_num],rect_Points[(draw_num+1)%4],Scalar(rand()%255,rand()%255,rand()%255),4);
//        }
//    }
    //绘制矩形
    for(Armor index_Armor:found_Armor)
    {
        index_Armor.GetCenterRectPoints(rect_Points);
        circle(final_Frame,index_Armor.getBoxCenter(),4,Scalar(rand()%255,rand()%255,rand()%255),3);
        for(int draw_num=0;draw_num<4;++draw_num)
        {
            //绘制边
            line(final_Frame,rect_Points[draw_num],rect_Points[(draw_num+1)%4],Scalar(255,255,255),2);
            //绘制顶点
            circle(final_Frame,rect_Points[draw_num], 4,Scalar (123,123,255),-1);
            circle(final_Frame,rect_Points[0], 4,Scalar (0,255,0),-1);
            circle(final_Frame,rect_Points[1], 4,Scalar (255,255,0),-1);
        }
    }
}

void FindModule::LightBarTOArmor(vector<LightBar> &input,vector<Armor> &output)
{

    if(input.size()<=1)return;
    int index = input.size()-1;
    LightBar test = input[index];
    for(int jugde = index-1;jugde>=0;--jugde)
    {
        LightBar test1= input[jugde];
        if(Armorjudge(test,test1))
        {
            //成功
            input.erase(input.end());
            input.erase(input.begin()+jugde);
            output.push_back(Armor(test,test1));
            break;
        }
    }
    LightBarTOArmor(input,output);
}

COLOR FindModule::GetRectColor(RotatedRect &_Rect) {
    //测试用，未实现
    return RED;
}

//这个函数传入两个灯条，判断他是否符合构造一个装甲版，如果可以，则返回真
bool FindModule::Armorjudge(const LightBar &left_bar , const LightBar &right_bar ) {

    return true;
}

float FindModule::getLbtoRbLenght(LightBar Left_, LightBar Right_) {
    float Lenght = 0;
    float width =abs(Right_.getCenter().x-Left_.getCenter().x);
    float height =abs(Left_.getCenter().y-Right_.getCenter().y);
    return sqrt(width*width+height*height);
}

void FindModule::showTargetNumber() {
    Mat background = Mat::zeros(Size(28*3,28*3), CV_8UC3);
    for(int index = 0 ;index<found_Armor.size();++index)
    {
        if(index>9)break;
        Mat roi= found_Armor[index].DisplayBox(src_Frame);
        Rect roi_rect = cv::Rect(index*28, 0, roi.cols, roi.rows);
        roi.copyTo(background(roi_rect));
    }
    namedWindow("target",WINDOW_NORMAL);
    imshow("target",background);
}
