#include <vector>
#include "ClockDetection.h"

using namespace std;
using namespace cv;


ClockDetection::ClockDetection(const string& imagePath) : image_path(imagePath) {}

cv::Mat ClockDetection::convertToGray(const cv::Mat& inputImage) {
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}


vector<Vec3f> ClockDetection::detectCircles(const cv::Mat& grayScale){

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(grayScale, circles, cv::HOUGH_GRADIENT, 1,
                     grayScale.rows / 8,  
                     100, 30, 10, 200  // the 10 and 200 value represent the range of circle sizes I suspect we are dealing with.
                     //I wasn't really sure what size to make them but I'm assuming we won't have any images of a clock with a circle
                     //smaller than 10 or larger than 200 - we can toy with this.
    );

    //DRAWS RED CIRCLE ON DETECTED CIRCLE:
    cv::Mat colorImage;
    cv::cvtColor(grayScale, colorImage, cv::COLOR_GRAY2BGR);
    for (const auto& circle : circles) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(colorImage, center, radius, cv::Scalar(0, 0, 255), 2); //draw red circle on gray image
    }

    //testing image output:
    imshow("Draw Copy", colorImage);
    int n = waitKey(0); // Wait for a keystroke in the window

    return circles;

}
