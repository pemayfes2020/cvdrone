#include "ardrone.h"
#include <iostream>
#include <vector>
// The code decoding H.264 video is based on the following sites.
// - An ffmpeg and SDL Tutorial - Tutorial 01: Making Screencaps -
//   http://dranger.com/ffmpeg/tutorial01.html
// - AR.Drone Development - 2.1.2 AR.Drone 2.0 Video Decording: FFMPEG + SDL2.0 -
//   http://ardrone-ailab-u-tokyo.blogspot.jp/2012/07/212-ardrone-20-video-decording-ffmpeg.html

// --------------------------------------------------------------------------
//! @brief   Initialize video.
//! @return  Result of initialization
//! @retval  1 Success
//! @retval  0 Failure
// --------------------------------------------------------------------------

#define  LOW_HUE        85            //hueの下限
#define  UP_HUE         110              //hueの上限
#define  LOW_SATURATION 60              //saturation（彩度）の下限
#define  LOW_VALUE      50              //value（明度）の下限

std::vector<cv::Vec3f> ARDrone::color_tracking(cv::Mat image, double &target_x, double &target_y, double &target_z){
   if(1){
    // cv::Mat output;
    // cv::inRange(image, output, );
    //std::vector<cv::Vec3f> c;
	// return c;

    // int hueMin = 0;
    // int hueMax = 33;
    // int saturation = 10;
    // int brightness = 10;

    //cv::Mat hsv;
    //cv::Mat result;
    // // カラー画像をHSV画像に変換
    // cv::cvtColor(image, hsb, CV_RGB2HSV, 3);
    // // inRangeによるフィルタ
    // cv::inRange(hsb, cv::Scalar(hueMin, saturation, brightness, 0) , cv::Scalar(hueMax, 255, 255, 0), result);
    
    //http://koshinran.hateblo.jp/entry/2017/10/03/193925色抽出

    // auto lower = cv::Scalar(0, 0, 200);
    // auto upper = cv::Scalar(50, 50, 255);

    // cv::inRange(image, lower, upper, result);
   }

    //http://carnation.is.konan-u.ac.jp/prezemi-1round/colorextraction.htm
    cv::Mat hsv, frame, hue, hue1, hue2, saturation, value, hue_saturation, image_black_white;  

    cv::cvtColor(image, hsv, CV_BGR2HSV);
    cv::GaussianBlur(image, image, cv::Size(9,9), 2, 2);


    std::vector<cv::Mat> singlechannels;//Matクラスのベクトルとしてsinglechannelsを定義

    cv::split(hsv, singlechannels);//hsvをsinglechannelsに分解([0]:h, [1]:s,[2]:v)

    //それぞれのチャンネルことに閾値を設定して二値化
    cv::threshold(singlechannels[0], hue1, LOW_HUE, 255, CV_THRESH_BINARY);                 // singlechannels[0]をLOW_HUEを閾値処理して、LOW_HUE以上の部分が255,それ以下の部分が0になるように、hue1に格納する。
    cv::threshold(singlechannels[0], hue2, UP_HUE, 255, CV_THRESH_BINARY_INV);              // singlechannels[0]をUP_HUEを閾値処理して、UP_HUE以上の部分が0,それ以下の部分が255になるように、hue2に格納する。
    cv::threshold(singlechannels[1], saturation, LOW_SATURATION, 255, CV_THRESH_BINARY);    //彩度LOW_SATURATION以上
    cv::threshold(singlechannels[2], value, LOW_VALUE, 255, CV_THRESH_BINARY);              //明度LOW_VALUE以上
    //条件を満たした領域をoutに設定
    cv::bitwise_and(hue1, hue2, hue);
    cv::bitwise_and(hue, saturation, hue_saturation);                                       // hueとsaturationのbitごとのandをとる→hue_saturation
    cv::bitwise_and(hue_saturation, value, image_black_white);                              // hue_saturationとvalueのbitごとのandをとる→image_black_white
    
    //cv::Canny(image_black_white,image_black_white,125, 125);

    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(image_black_white, circles, CV_HOUGH_GRADIENT, 2, image_black_white.rows / 4.0, 200, 100, 0, 80);

    std::vector<cv::Vec3f> ret_circle;

    for(size_t i = 0;i < circles.size();i++){
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radious = cvRound(circles[i][2]);

        cv::circle(image, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
        cv::circle(image, center, radious, cv::Scalar(0, 0, 255), 3, 8, 0);
        ret_circle.push_back(circles[i]);   
	}

    cv::namedWindow("image_black_white");
    cv::imshow("image_black_white",image_black_white);

    cv::namedWindow("image");
    cv::imshow("image",image);

    return ret_circle;
}
