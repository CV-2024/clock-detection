#ifndef CLOCK_DETECTION_H
#define CLOCK_DETECTION_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ClockDetection
{
private:
    string image_path;

public:
    // Default constructor
    ClockDetection(const string &imagePath);
    /*
    @breif convert colored image to greyscale
    @param const cv::Mat& - color image
    */
    cv::Mat convertToGray(const cv::Mat &inputImage);

        /*
        @brief Detect circles in a grayscale image using Hough Circle Transformation
        @param grayImage - Grayscale image
        @param dp - a ingter that is an inverse ratio. It essentially controls the resolution of the accumulator relative to the resolution of the input image. 
        @param minDist - a ingter that specifies the minimum distance between the centers of the detected circles.
        @param param1 - a ingter that is the higher threshold,for HOUGH_GRADIENT. The lower threshold is twice smaller.
        @param param2 - a ingter that is the accumulator threshold for the circle centers at the detection stage.
        @param minRadius - Minimum radius to start the search
        @return Detected circles (center coordinates and radius)
        */
       vector<Vec3f> detectCircles(const cv::Mat& grayImage, int dp, int param1, int param2, int minDist, int minRadius, int maxRadius);

       /*
        @brief Draw the Circle on the passed image directly
        @param Image - input image to be drawn on 
        @param circles - vector of Detected circles(center coordinates and radius)
       */
       void drawDetectCircles(vector<Vec3f> circles, const cv::Mat& Image);
       /*
        @brief Draw the Circle on the copy of the passed image and display it.
        @param grayImage - Grayscale image
        @param circles - vector of Detected circles(center coordinates and radius)
       */
       void drawDetectCirclesCopy(vector<Vec3f> circles, const cv::Mat& grayImage);

    /*
    @brief Detect ellipse in a grayscale image
    @param const cv::Mat& - grayscale image
    @return vector<cv::Vec3f> - detected ellipse (center coordinates and radius)
    */
    void detectEllipse(const cv::Mat &grayImage);

};

#endif /* CLOCK_DETECTION_H */