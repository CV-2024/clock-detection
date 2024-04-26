#include "ClockDetection.h"

ClockDetection::ClockDetection(const string &imagePath) : image_path(imagePath) {}

cv::Mat ClockDetection::convertToGray(const cv::Mat &inputImage)
{
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}

vector<Vec3f> ClockDetection::detectCircles(const cv::Mat& grayImage, int dp, int param1, int param2, int minDist, int minRadius, int maxRadius){
    vector<Vec3f> circles;
    // Detect circles using Hough Circle Transformation
    HoughCircles(grayImage, circles, HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
    return circles;
}


void ClockDetection::drawDetectCirclesCopy(vector<Vec3f> circles, const cv::Mat& grayImage){
    //DRAWS RED CIRCLE ON DETECTED CIRCLE:
    cv::Mat colorImage;

    cv::cvtColor(grayImage, colorImage, cv::COLOR_GRAY2BGR);
    for (const auto& circle : circles) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(colorImage, center, radius, cv::Scalar(0, 0, 255), 2); //draw red circle on gray image
    }

    // //testing image output:
    imshow("Draw Copy", colorImage);
    int n = waitKey(0); // Wait for a keystroke in the window
}

void ClockDetection::drawDetectCircles(vector<Vec3f> circles, const cv::Mat& Image){
    for (const auto& circle : circles) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(Image, center, radius, cv::Scalar(0, 0, 255), 2); //draw red circle on gray image
    }
}


void ClockDetection::detectEllipse(const cv::Mat &grayImage)
{
    // Detect edges using Canny edge detector
    Mat edges;
    //params: image, edges array, threshold1, threshold2
    Canny(grayImage, edges, 50, 150);

    // Find contours
    vector<vector<Point>> contours;
    //params: image, output contours, mode, method
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//returns a modified image

    // Filter contours based on area or other criteria
    vector<Point> largestContour;
    double maxArea = 0;
    for (const auto& contour : contours) {
        double area = contourArea(contour);//Computes Area of the contours
        //Finds max area
        if (area > maxArea) {
            maxArea = area;
            largestContour = contour;
        }
    }

    // Approximate contour to reduce the number of points
    vector<Point> approx;
    double epsilon = 0.01 * arcLength(largestContour, true);
    //params: array of curves, output array, epsilon, bool closed or not
    approxPolyDP(largestContour, approx, epsilon, true);

    // Fits an ellipse around a set of 2D points.
    //params: input array of points
    RotatedRect ellipse = fitEllipse(approx);

    // Convert grayscale image to BGR
    Mat resultBGR;
    cvtColor(grayImage, resultBGR, COLOR_GRAY2BGR);

    // Draw ellipse on the grayscale image
    cv::ellipse(resultBGR, ellipse, Scalar(0, 255, 0), 2);

    // Get center and radii of the ellipse
    Point2f center = ellipse.center;
    float radiusX = ellipse.size.width / 2;
    float radiusY = ellipse.size.height / 2;

    // Draw red dot at the center of the ellipse
    circle(resultBGR, center, 5, Scalar(0, 0, 255), -1); // -1 indicates filled circle

    // Show the result
    imshow("Detected Ellipse", resultBGR);
    waitKey(0);
}


