#ifndef CLOCK_DETECTION_H
#define CLOCK_DETECTION_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ClockDetection {
    private:
        string image_path;
    public:
        // Default constructor
        ClockDetection(const string& imagePath);
        /*
        @breif convert colored image to greyscale
        @param const cv::Mat& - color image 
        */
        cv::Mat convertToGray(const cv::Mat& inputImage);

        /*
        @brief Detect circles in a grayscale image using Hough Circle Transformation
        @param const cv::Mat& - grayscale image
        @return vector<cv::Vec3f> - detected circles (center coordinates and radius)
        */
        vector<Vec3f> detectCircles(const cv::Mat& grayScale);

};

#endif /* CLOCK_DETECTION_H */