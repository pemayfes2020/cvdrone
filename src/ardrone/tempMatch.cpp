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
using namespace cv;
using namespace std;

#define  LOW_SATURATION 65              //saturation（彩度）の下限
#define  LOW_VALUE      50              //value（明度）の下限

std::vector<Vec3f> ARDrone::detectCircle(cv::Mat image, double &target_x, double &target_y, double &target_z, int LOW_HUE, int UP_HUE){
    //ref = http://opencv.jp/opencv-2svn/cpp/feature_detection.html
	 //http://carnation.is.konan-u.ac.jp/prezemi-1round/colorextraction.htm
    cv::Mat hsv, frame, hue, hue1, hue2, saturation, value, hue_saturation, image_black_white;  

    std::vector<cv::Vec3f> circles;

	Mat img = image.clone();
    cv::cvtColor(image, hsv, CV_BGR2HSV);
    //cv::GaussianBlur(img, img, cv::Size(9,9), 2, 2);
	//cv::bilateralFilter(img, img, 10, 100, 10, cv::BORDER_DEFAULT);

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


	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(image_black_white, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f> center( contours.size() );
 	vector<float> radius( contours.size() );

	for( int i = 0; i < contours.size(); i++ ){
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
		double L = arcLength(contours[i], true);
		double S = contourArea(contours[i]);
		double circle_label = 4 * M_PI * S / (L * L);
	    if(circle_label > 0.55 && radius[i] > 5.0)circle(image, center[i], (int)radius[i], cv::Scalar(0, 0, 255), 2, 8, 0 );
    }


    // cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 2, CV_AA);

    // トップレベルにあるすべての輪郭を横断し，
    // 各連結成分をランダムな色で描きます．
    // int idx = 0;
    // for( ; idx >= 0; idx = hierarchy[idx][0] )
    // {
    //     Scalar color(255, 0, 0);
    //     drawContours(image, contours, idx, color, CV_FILLED, 8, hierarchy );
    // }

	cv::namedWindow("image_black_white");
    cv::imshow("image_black_white",image_black_white);

    cv::namedWindow("image");
    cv::imshow("image",image);
	return circles;
}