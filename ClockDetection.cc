#include "ClockDetection.h"

ClockDetection::ClockDetection(const string &imagePath) : image_path(imagePath) {}

cv::Mat ClockDetection::convertToGray(const cv::Mat &inputImage)
{
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}

vector<Vec3f> ClockDetection::detectCircles(const cv::Mat &grayImage)
{
}

void ClockDetection::detectEllipse(const cv::Mat &grayImage)
{
    // Detect edges using Canny edge detector
    Mat edges;
    Canny(grayImage, edges, 50, 150);

    // Find contours
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Filter contours based on area or other criteria
    vector<Point> largestContour;
    double maxArea = 0;
    for (const auto& contour : contours) {
        double area = contourArea(contour);
        if (area > maxArea) {
            maxArea = area;
            largestContour = contour;
        }
    }

    // Approximate contour to reduce the number of points
    vector<Point> approx;
    double epsilon = 0.01 * arcLength(largestContour, true);
    approxPolyDP(largestContour, approx, epsilon, true);

    // Fit ellipse to the contour
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