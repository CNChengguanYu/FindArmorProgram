
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

    threshold(bin_Frame,dst_bin_Frame,250,255,THRESH_BINARY);

    //开闭
    dilate(dst_bin_Frame,dst_bin_Frame, getStructuringElement(MORPH_RECT,Size(11,11)));
    erode(dst_bin_Frame,dst_bin_Frame,getStructuringElement(MORPH_RECT, Size(5, 5)));
    //dilate(dst_bin_Frame,dst_bin_Frame, getStructuringElement(MORPH_RECT,Size(3,3)));

    imshow("dst_bin",dst_bin_Frame);


    findContours(dst_bin_Frame,contours,hierarchy,RETR_CCOMP, CHAIN_APPROX_NONE);

    if(contoursTOLightBar())cout<<"找到灯条"<<found_LightBar.size()<<endl;
    for(LightBar index:found_LightBar)
    {
        float angle =index.getAngle();
        putText(final_Frame, to_string((int)angle),index.getCenter(),2,1,Scalar(150,167,189));
    }
    LightBarTOArmor(found_LightBar,found_Armor);

    Drawrects();

    //cout<<"装甲版"<<found_Armor.size()<<"个"<<endl;

    //showTargetNumber();

    imshow("test",final_Frame);
    std::cout<<"帧结束"<<std::endl;
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
       if(get_Rect.size.area()<MIN_LIGHTBAR_AREA || get_Rect.size.area()>MAX_LIGHTBAR_AREA)continue;
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
    Point2f l_Light_Points[4]={};
    Point2f r_Light_Points[4]={};

    Scalar color;
    //绘制矩形
    for(Armor index_Armor:found_Armor)
    {
        if(index_Armor.getColor()==RED){color=Scalar(50,50,255);}
            else if(index_Armor.getColor()==BLUE){color=Scalar(255,50,50);}
            else{color=Scalar(255,255,255);}

        index_Armor.GetCenterRectPoints(rect_Points);
        index_Armor.getLeftLightBar().getRect().points(l_Light_Points);
        index_Armor.getRightLightBar().getRect().points(r_Light_Points);

        circle(final_Frame,index_Armor.getBoxCenter(),4,Scalar(rand()%255,rand()%255,rand()%255),3);
        for(int draw_num=0;draw_num<4;++draw_num)
        {
            //绘制边
            line(final_Frame,rect_Points[draw_num],rect_Points[(draw_num+1)%4],color,4);
            //绘制顶点
            circle(final_Frame,rect_Points[draw_num], 4,Scalar (123,123,255),-1);

            //绘制左边灯条
            line(final_Frame,l_Light_Points[draw_num],l_Light_Points[(draw_num+1)%4],color,4);
            //右边
            line(final_Frame,r_Light_Points[draw_num],r_Light_Points[(draw_num+1)%4],color,4);

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
            input.erase(input.begin()+jugde);
            output.push_back(Armor(test,test1));
            break;
        }
    }
    //匹配完成后无论是否成功都应该退出末位，否则下一次递归将会重复判断该灯条
    input.erase(input.end());
    //递归调用
    LightBarTOArmor(input,output);
}

COLOR FindModule::GetRectColor(RotatedRect &_Rect) {
    Mat ROI;
    int red_color=0;
    int blue_color=0;
    Rect ROI_Rect(_Rect.boundingRect());
    ROI_Rect.width+=fmax(10,_Rect.size.width*0.3);
    ROI_Rect.height+=fmax(20,_Rect.size.height*0.3);

    if(src_Frame.cols<=ROI_Rect.x+ROI_Rect.width)ROI_Rect.width=src_Frame.cols-ROI_Rect.x-2;

    if(src_Frame.rows<=ROI_Rect.y+ROI_Rect.height)ROI_Rect.height=src_Frame.rows-ROI_Rect.y-2;


    if(ROI_Rect.x<0)ROI_Rect.x=0;
    if(ROI_Rect.y<0)ROI_Rect.y=0;


    if(ROI_Rect.empty() || ROI_Rect.area()< MIN_LIGHTBAR_AREA){return NORMAL;}

    src_Frame(ROI_Rect).copyTo(ROI);

    Mat_<Vec3b>::iterator it = ROI.begin<Vec3b>();
    Mat_<Vec3b>::iterator itend = ROI.end<Vec3b>();

    for(;it!=itend;++it)
    {
       blue_color+=(*it)[0];
       red_color += (*it)[2];
    }
    imshow("roi",ROI);
    if(blue_color>red_color*1.1){return BLUE;}else if(red_color*1.1>blue_color){return RED;}else{return NORMAL;}
}

//这个函数传入两个灯条，判断他是否符合构造一个装甲版，如果可以，则返回真
bool FindModule::Armorjudge(LightBar &left_bar ,LightBar &right_bar ) {
    //如果角度差大于设定值，则认为两个灯条不匹配
    if(!judgeLightBarAngle(left_bar,right_bar)){std::cout<<"匹配失败！[角度]"<<std::endl;return false;}
    if(!judgeLightBarDistance(left_bar,right_bar)){std::cout<<"匹配失败！[距离]"<<std::endl;return false;}
    if(!judgeLightBarHeightDiff(left_bar,right_bar)){std::cout<<"匹配失败！[角度-高度差]"<<std::endl;return false;}
    if((left_bar.getColor()!=right_bar.getColor())){std::cout<<"匹配失败！[颜色]"<<std::endl;return false;}
    return true;
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

float FindModule::getLBtoRBlength(LightBar &left_Bar , LightBar &right_Bar) {
    float width = abs(left_Bar.getCenter().x-right_Bar.getCenter().x);
    float height = abs(left_Bar.getCenter().y-right_Bar.getCenter().y);
    return sqrt(width*width+height*height);
}

bool FindModule::judgeLightBarAngle(LightBar &left_Bar, LightBar &right_Bar) {
    float angle_diff = abs(left_Bar.getAngle()-right_Bar.getAngle());
    if(angle_diff > ANGLE_THRESHOLD){std::cout<<"当前角度差"<<angle_diff<<"，左右："<<left_Bar.getAngle()<<","<<right_Bar.getAngle()<<std::endl;return false;}
    return true;
}

bool FindModule::judgeLightBarDistance(LightBar &left_Bar, LightBar &right_Bar) {
    float lenght =getLBtoRBlength(left_Bar,right_Bar);
    //装甲板宽度 = 灯条长度*2 ，这里灯条长取平均值
    //         = （（左长+右长）/2）*2
    //         = 左长+右长
    float safe_lenght =left_Bar.getHeight()+right_Bar.getHeight();
    if(lenght <safe_lenght*1.5 && lenght>safe_lenght*0.7){
        return true;
    }else{
        return false;
    }
}

bool FindModule::judgeLightBarHeightDiff(LightBar &left_Bar, LightBar &right_Bar) {
    //以灯条的左-右中心为斜边，构造直角三角形
    //
    float Borderline = abs(left_Bar.getCenter().x-right_Bar.getCenter().x);
    float hypotenuse = getLBtoRBlength(left_Bar,right_Bar);
    float ang = Borderline/hypotenuse;
    if(ang>0.7)
    {
        std::cout<<"angle=true==>"<<ang<<"B/R"<<Borderline<<"/"<<hypotenuse<<std::endl;
        return true;
    }else
    {
        std::cout<<"angle====>"<<ang<<std::endl;
        return false;
    }
}
