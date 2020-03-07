#include "ardrone.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

void floordetect(cv::Mat src, cv::Mat *image){
    cv::Mat gray, inv, dst;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);  //グレースケールにして反転
    cv::bitwise_not(gray, inv);

    cv::OutputArray mylines;
    double rho = 1.0;
    double theta = M_PI/360;
    int threshold = 80;
    double minLineLength = 80;
    double maxLineGap = 5;
    cv::HoughLinesP(buffer, mylines, rho, theta, threshold, minLineLength, maxLineGap);

    for(auto line : lines){
        cv::line(dst, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 1);
    }

    cv::namedWindow("dst");
    cv::imshow("dst", dst);
    cv::moveWindows("dst", 0, 500);

    cv::waitKey(0);   

}