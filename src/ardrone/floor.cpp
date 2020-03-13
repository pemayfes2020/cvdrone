#include "ardrone.h"
#include <iostream>
#include <vector>
#include <algorithm>

void ARDrone::floorDetect(cv::Mat src){
    cv::Mat gray, inv, dst, cann;
    dst = src.clone();
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);  //グレースケールにして反転
    cv::bitwise_not(gray, inv);
    cv::Canny(inv, cann, 10, 100, 3);

    cv::namedWindow("inv");
    cv::imshow("inv", cann);
    cv::moveWindow("inv", 640, 360);

    std::vector<cv::Vec4i> mylines;
    double rho = 1.0;
    double theta = CV_PI/360;
    int threshold = 250;
    double minLineLength = 200;
    double maxLineGap = 500;
    cv::HoughLinesP(cann, mylines, rho, theta, threshold, minLineLength, maxLineGap);
/*
    std::vector<cv::Vec2f>::iterator it = mylines.begin();
    for(; it!=mylines.end(); ++it){
        cv::Point pt1, pt2;
        double a = cos((*it)[1]);
        double b = sin((*it)[1]);
        double x0 = a * (*it)[0];
        double y0 = b * (*it)[0];
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*a);
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*a);
        cv::line(dst, pt1, pt2, cv::Scalar(0, 0, 255), 1, CV_AA);
    }
*/
    //std::cout << mylines.size() << std::endl;

    for(size_t i = 0; i < mylines.size(); ++i){
        cv::Vec4i l = mylines[i];
        cv::Point pt1, pt2;
        pt1.x = l[0];
        pt1.y = l[1];
        pt2.x = l[2];
        pt2.y = l[3];
        //std::cout << pt1 << pt2 << std::endl;
        //std::cout << src.cols<< src.rows << std::endl;
        cv::line(dst, pt1, pt2, cv::Scalar(0, 0, 255), 1);
    }

    cv::namedWindow("dst");
    cv::imshow("dst", dst);
    cv::moveWindow("dst", 0, 500);


}