
#include "ClockDetection.h"

cv::Mat ClockDetection::convertToGray(const cv::Mat& inputImage) {
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}
