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


vector<Vec3f> ARDrone::detectCircle(Mat image, double &target_x, double &target_y, double &target_z){
    //ref = http://opencv.jp/opencv-2svn/cpp/feature_detection.html

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    GaussianBlur(gray, gray, Size(9,9), 2, 2);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, gray.rows / 32, 200, 100);

	/************************************************************************* 
	circlesはrow x ３の2次元vector (float)
	circles[row][0]: row番目の円のx座標
    circles[row][1]: row番目の円のy座標
	circles[row][2]: row番目の円の半径 

	!!openCVにおいて、画素値の順番は<B, G, R>!!	
	**************************************************************************/

	vector<Vec3f> ret_circle;

//	Vec3f ptImg = image;

    for(size_t i = 0;i < circles.size();i++){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radious = cvRound(circles[i][2]);

     	//Vec3f* ptr = image.ptr<Vec3f>(center.x);  これできれば早いらしい

		//for(int j = -1;j < 2;j++){
		//	for(int k = -1;k < 2;k++){
   		//		flag = (flag || );	
		//	}
		//}
		//if(ptImg(center)[2] > 50 && ptImg(center)[1] < 30 && ptImg(center)[0] < 30){
   			 //cout << ptImg(center) << endl;
		//if(flag){	
		 //	cout << ptImg(center)<< endl;  
     		circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
   		    circle(image, center, radious, Scalar(0, 0, 255), 3, 8, 0);
	    	ret_circle.push_back(circles[i]);    	
		//}
	}

	imshow("camera", image);	 
				

    return ret_circle;
}
