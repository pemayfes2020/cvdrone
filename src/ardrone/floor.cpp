#include "ardrone.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

void ARDrone::floorDetect(cv::Mat src){
    cv::Mat gray, inv, dst;
    dst = src.clone();
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);  //グレースケールにして反転
    cv::bitwise_not(gray, inv);

    std::vector<cv::Vec2d> mylines;
    double rho = 1.0;
    double theta = M_PI/360;
    int threshold = 80;
    double minLineLength = 80;
    double maxLineGap = 5;
    cv::HoughLinesP(inv, mylines, rho, theta, threshold, minLineLength, maxLineGap);

    std::vector<cv::Vec2d>::iterator it = mylines.begin();
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
        cv::line(dst, pt1, pt2, cv::Scalar(0, 0, 255), 1);
    }

    cv::namedWindow("dst");
    cv::imshow("dst", dst);
    cv::moveWindow("dst", 0, 500);

    cv::waitKey(0);   

}