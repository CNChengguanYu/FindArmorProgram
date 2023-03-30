/*此模块为查找类的函数实现*/
#include "FindModule.h"
bool FindModule::find(Mat &src) {
    /*通过输入的矩阵来查找图中灯条*/

    std::cout<<"---===帧开始===---"<<std::endl;
    if(src.empty()){
        //如果图空，则返回假
        return false;
    }
    //清空查找到的装甲版，防止留下上一帧的结果
    found_Armor.clear();
    found_LightBar.clear();

    //设置原始图像（作为ROI的模板）
    src.copyTo(src_Frame);
    //设置输出图像
    src.copyTo(final_Frame);
    //BGR转换为灰度图
    cvtColor(src,bin_Frame,COLOR_BGR2GRAY);
    //二值处理
    threshold(bin_Frame,dst_bin_Frame,220,255,THRESH_BINARY);
    //打开二值图像
    //imshow("bin_frame",bin_Frame);

    //开闭
    dilate(dst_bin_Frame,dst_bin_Frame, getStructuringElement(MORPH_RECT,Size(11,11)));
    erode(dst_bin_Frame,dst_bin_Frame,getStructuringElement(MORPH_RECT, Size(5, 5)));
    //dilate(dst_bin_Frame,dst_bin_Frame, getStructuringElement(MORPH_RECT,Size(3,3)));

    //开闭后的二值图像
    //imshow("dst_bin",dst_bin_Frame);

    //查找边缘
    findContours(dst_bin_Frame,contours,hierarchy,RETR_CCOMP, CHAIN_APPROX_NONE);

    //如果返回值<2则退出（无法构造成装甲版）
    if(!contoursTOLightBar())//return false;

    cout<<"找到灯条"<<found_LightBar.size()<<endl;
    //输入找到的灯条队列，匹配至装甲板队列
    LightBarTOArmor(found_LightBar,found_Armor);
    //cout<<"装甲版"<<found_Armor.size()<<"个"<<endl;

    //绘制图形
    Drawrects();

    //显示装甲板中心区域的函数
    showTargetNumber();

    //输出最终图像
    imshow("test",final_Frame);

    std::cout<<"---===帧结束===---"<<std::endl;
    return true;
}

FindModule::FindModule(COLOR color) {
    /*查找器的构造函数*/
    //设置颜色
    mode = color ;
}

bool FindModule::contoursTOLightBar() {
    /*从轮廓构造灯条*/

    //根据画面轮廓构造灯条：遍历所有轮廓
    for(vector<Point> index_contours:contours)
    {
        //如果轮廓点数<6（无法拟合椭圆）则跳过该轮廓
       if(index_contours.size()<6)continue;
       //新建旋转矩形
       RotatedRect get_Rect;
       //从椭圆构造矩形,在方向上比从边缘构建最小外接矩形更准确
       get_Rect = fitEllipse(index_contours);
       //过滤面积不符合则剔除（减少多余运算）
       if(get_Rect.size.area()<MIN_LIGHTBAR_AREA || get_Rect.size.area()>MAX_LIGHTBAR_AREA)continue;
       //构造灯条
       LightBar temp_lightbar=LightBar(GetRectColor(get_Rect),get_Rect);
       //判断角度，如果角度不符合则剔除
       if(abs(temp_lightbar.getAngle())>MAX_LIGHTBAR_ANGLE)continue;
       //所有条件符合，装入队列中
       found_LightBar.push_back(temp_lightbar);
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
    /*绘制图像到输出帧*/

    //装甲中心顶点
    Point2f rect_Points[4]={};
    //左灯条顶点
    Point2f l_Light_Points[4]={};
    //右灯条顶点
    Point2f r_Light_Points[4]={};
    //绘制颜色
    Scalar color;
    //遍历所有装甲板
    for(Armor index_Armor:found_Armor)
    {   //根据装甲板颜色设置绘制颜色
        if(index_Armor.getColor()==RED){color=Scalar(50,50,255);}
            else if(index_Armor.getColor()==BLUE){color=Scalar(255,50,50);}
            else{color=Scalar(255,255,255);}

        //逐个获取中心、左、右四点
        index_Armor.getArmorBoxPoints(rect_Points);
        index_Armor.getLeftLightBar().getRect().points(l_Light_Points);
        index_Armor.getRightLightBar().getRect().points(r_Light_Points);
        //绘制圆到装甲板中心
        circle(final_Frame,index_Armor.getBoxCenter(),4,Scalar(rand()%255,rand()%255,rand()%255),3);
        //绘制矩形
        for(int draw_num=0;draw_num<4;++draw_num)
        {
            //绘制中心矩形边
            line(final_Frame,rect_Points[draw_num],rect_Points[(draw_num+1)%4],color,4);
            //绘制中心四边顶点
            circle(final_Frame,rect_Points[draw_num], 4,Scalar (123,123,255),-1);
            //绘制左边灯条
            line(final_Frame,l_Light_Points[draw_num],l_Light_Points[(draw_num+1)%4],color,4);
            //右边
            line(final_Frame,r_Light_Points[draw_num],r_Light_Points[(draw_num+1)%4],color,4);
        }
    }
}

void FindModule::LightBarTOArmor(vector<LightBar> &input,vector<Armor> &output){
    /*从灯条匹配到装甲板*/

    //如果灯条数小于等于1，则返回
    if(input.size()<=1)return;
    //设置下标为元素数-1（如数组一样，int arr[10]有10个元素，但是下标范围为0-9），从队列尾端开始匹配
    int index = input.size()-1;
    //取出尾端元素追踪，开始和其他灯条匹配
    LightBar Targeting = input[index];
    for(int judge = index-1;judge>=0;--judge)
    {
        //取出尾端-1的元素，和追踪目标匹配
        LightBar mate= input[judge];
        //执行Armorjudge函数，判断两个灯条是否匹配
        if(Armorjudge(Targeting,mate))
        {
            //成功
            //从队列中删除该元素（mate）
            input.erase(input.begin()+judge);
            //将找到的灯条构造成装甲板，存入装甲板队列
            output.push_back(Armor(Targeting,mate));
            //终止循环
            break;
        }
        //这一个灯条无法匹配，则进入下一次循环
    }
    //匹配完成
    //匹配完成后无论是否成功（Targeting）都应该退出末位，否则下一次递归将会重复判断该灯条
    input.erase(input.end());
    //递归调用自己
    LightBarTOArmor(input,output);
}

COLOR FindModule::GetRectColor(RotatedRect &_Rect) {
    /*获取灯条区域颜色*/

    //定义两颜色的强度值
    red_color=0;blue_color=0;
    //从灯条区域构造矩形
    Rect ROI_Rect(_Rect.boundingRect());
    //扩大矩形选区，取最大值
    ROI_Rect.width+=fmax(10,_Rect.size.width*0.3);
    ROI_Rect.height+=fmax(20,_Rect.size.height*0.3);

    //判断矩形+坐标的值是否超界
    if(src_Frame.cols<=ROI_Rect.x+ROI_Rect.width)ROI_Rect.width=src_Frame.cols-ROI_Rect.x-2;
    if(src_Frame.rows<=ROI_Rect.y+ROI_Rect.height)ROI_Rect.height=src_Frame.rows-ROI_Rect.y-2;

    //判断矩形是否超界，如果矩形的值超过了边界，则设置为边界值
    if(ROI_Rect.x<0)ROI_Rect.x=0;
  //  if(ROI_Rect.width>src_Frame.cols)ROI_Rect.width=src_Frame.cols;
    if(ROI_Rect.width>src_Frame.cols)ROI_Rect.width=0;
    if(ROI_Rect.y<0)ROI_Rect.y=0;
  //  if(ROI_Rect.height>src_Frame.rows)ROI_Rect.height=src_Frame.rows;
    if(ROI_Rect.height>src_Frame.rows)ROI_Rect.height=0;
    if(ROI_Rect.x<0)ROI_Rect.x=0;

    //如果矩形的面积太小，则不判断（信噪比太高，判断无意义）
    if(ROI_Rect.empty() || ROI_Rect.area()< MIN_LIGHTBAR_AREA){return NORMAL;}

    //矩形设置完毕，以矩形为MASK设置ROI区域
    src_Frame(ROI_Rect).copyTo(ROI);

    //设置迭代器
    Mat_<Vec3b>::iterator it = ROI.begin<Vec3b>();
    Mat_<Vec3b>::iterator itend = ROI.end<Vec3b>();
    //遍历迭代器（图中所有像素点）
    for(;it!=itend;++it)
    {
        //累加该像素的值
       blue_color+=(*it)[0];
       red_color += (*it)[2];
    }
    //遍历过后得到各分量强度
    //由于在摄像头中红色灯条会偏紫色，蓝色分量可能过高，红色较低，因此乘以权重1.1
    //如果蓝色分量大于红色分量*1.1，则认为是蓝色灯条，以此类推
    if(blue_color>red_color*1.1){return BLUE;}else if(red_color*1.1>blue_color){return RED;}else{return NORMAL;}
}


bool FindModule::Armorjudge(LightBar &left_bar ,LightBar &right_bar ) {
    /*该函数判断两个灯条是否能够构成一个装甲板
     * 如果可以，则返回真*/

    //角度判别（两灯条的角度差）
    if(!judgeLightBarAngle(left_bar,right_bar)){std::cout<<"匹配失败！[角度]"<<std::endl;return false;}
    //距离判别（两灯条的距离不能太大）
    if(!judgeLightBarDistance(left_bar,right_bar)){std::cout<<"匹配失败！[距离]"<<std::endl;return false;}
    //灯条高度差判别（两灯条的高度差是否太大）
    if(!judgeLightHeight(left_bar,right_bar))return false;
    //颜色判别
    if((left_bar.getColor()!=right_bar.getColor())){std::cout<<"匹配失败！[颜色]"<<std::endl;return false;}
    //同角度下高差判别（防止匹配到平行但是形变的灯条）
    if(!judgeLightBarHeightDiff(left_bar,right_bar)){std::cout<<"匹配失败！[角度-高度差]"<<std::endl;return false;}
    //匹配成功
    return true;
}

void FindModule::showTargetNumber() {
    /*展示获取到的装甲板标签图片*/

    //创建一个纯黑背景，可以容纳3*3=9个图片
    Mat background = Mat::zeros(Size(DST_TARGET_AREA*3,DST_TARGET_AREA*3), CV_8UC3);
    //遍历装甲板
    for(int index = 0 ;index<found_Armor.size();++index)
    {
        //如果超过最大显示数，则退出
        if(index>9)break;
        //从装甲板传回ROI图像
        Mat roi= found_Armor[index].DisplayBox(src_Frame);
        //设置ROI区域在背景的位置
        Rect roi_rect = cv::Rect(index*DST_TARGET_AREA, index/3, roi.cols, roi.rows);
        //将ROI图像绘制在ROI区域上
        roi.copyTo(background(roi_rect));
    }
    //设置窗口
    namedWindow("target",WINDOW_NORMAL);
    //展示
    imshow("target",background);
}

float FindModule::getLBtoRBlength(LightBar &left_Bar , LightBar &right_Bar) {
    /*获取两灯条的距离
     * 返回 距离float*/

    //勾股定理
    float width = abs(left_Bar.getCenter().x-right_Bar.getCenter().x);
    float height = abs(left_Bar.getCenter().y-right_Bar.getCenter().y);
    return sqrt(width*width+height*height);
}

bool FindModule::judgeLightBarAngle(LightBar &left_Bar, LightBar &right_Bar) {
    /*判断两灯条的角度差
     * 返回 合格T/不合格F bool*/

    float angle_diff = abs(left_Bar.getAngle()-right_Bar.getAngle());
    //大于设置的阈值，则返回错误
    if(angle_diff > MAX_ANGLE_DIFF){std::cout << "当前角度差" << angle_diff << "，左右：" << left_Bar.getAngle() << "," << right_Bar.getAngle() << std::endl;return false;}
    return true;
}

bool FindModule::judgeLightBarDistance(LightBar &left_Bar, LightBar &right_Bar) {
    /*判断两灯条距离是否合规
     * 返回 合格T/不合格F bool*/

    float lenght =getLBtoRBlength(left_Bar,right_Bar);
    //装甲板宽度 = 灯条长度*2 ，这里灯条长取平均值
    //         = （（左长+右长）/2）*2
    //         = 左长+右长
    //设置安全长度
    float safe_lenght =left_Bar.getHeight()+right_Bar.getHeight();
    //两灯条距离是否在阈值内
    if(lenght <safe_lenght*MAX_DISTANCE_COEF
    && lenght>safe_lenght*MIN_DISTANCE_COEF){
        return true;
    }else{
        return false;
    }
}

bool FindModule::judgeLightBarHeightDiff(LightBar &left_Bar, LightBar &right_Bar) {
    /*判断两灯条的构成的直角三角形的角度差（具体原理请看/DOC路径下的同名图片文件）
     *返回 合格T/不合格F bool */

    //以灯条的左-右中心为斜边，构造直角三角形
    //三角形的邻边
    float Borderline = abs(left_Bar.getCenter().x-right_Bar.getCenter().x);
    //三角形的斜边
    float hypotenuse = getLBtoRBlength(left_Bar,right_Bar);
    //运用三角函数的知识，cos=邻边/斜边
    float cos = Borderline/hypotenuse;
    // 根据函数图像可得，角度越小越靠近1，这里设置为0.8，允许一定的角度
    if(cos>MAX_COS_VALUE){return true;}
    else{return false;}
}

bool FindModule::judgeLightHeight(LightBar &left_Bar, LightBar &right_Bar) {
    /*判断两灯条高度差
     * 返回 合格T/不合格F bool*/

    //获取左灯条
    float height =left_Bar.getHeight();
    //是否在阈值范围内
    if(height > right_Bar.getHeight()*MAX_LIGHTBAR_HEIGHT_DIFF_COEF
    || height <=right_Bar.getHeight()*MIN_LIGHTBAR_HEIGHT_DIFF_COEF)return false;
    return true;
}

FindModule::FindModule() {

}

FindModule::~FindModule() {

}
