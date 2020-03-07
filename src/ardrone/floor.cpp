#include "ardrone.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

void floordetect(cv::Mat image, cv::Mat *result){
    cv::Mat buffer = cv::bitwise_not(cv::cvtColor(image, cv::COLOR_BGR2GRAY));  //グレースケールにして反転
    cv::Mat buffer2 = cv::bitwise_not()

    cv::OutputArray mylines;
    double rho = 1.0;
    double theta = M_PI/360;
    int threshold = 80;
    double minLineLength = 80;
    double maxLineGap = 5;
    cv::HoughLinesP(buffer, mylines, rho, theta, threshold, minLineLength, maxLineGap);


    

}