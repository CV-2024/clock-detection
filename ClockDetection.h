#ifndef CLOCK_DETECTION_H
#define CLOCK_DETECTION_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ClockDetection {
public:
    /*
    @breif convert colored image to greyscale
    @param const cv::Mat& - color image 
    */
    cv::Mat convertToGray(const cv::Mat& inputImage);
};

#endif /* CLOCK_DETECTION_H */