#include "ClockDetection.h"

ClockDetection::ClockDetection(const string &imagePath) : image_path(imagePath) {}

cv::Mat ClockDetection::convertToGray(const cv::Mat &inputImage)
{
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}

vector<Vec3f> ClockDetection::detectCircles(const cv::Mat &grayImage, int dp, int param1, int param2, int minDist, int minRadius, int maxRadius)
{
    vector<Vec3f> circles;
    // Detect circles using Hough Circle Transformation
    HoughCircles(grayImage, circles, HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);

    // // Print out center coordinates detected for image input
    // for (const auto& circle : circles) {
    //     float x = circle[0];
    //     float y = circle[1];
    //     float radius = circle[2];
    //     std::cout << "Center: (" << x << ", " << y << "), Radius: " << radius << std::endl;
    // }

    return circles;
}

void ClockDetection::drawDetectCirclesCopy(string name, const vector<Vec3f>& circles, const cv::Mat& grayImage){
    //DRAWS RED CIRCLE ON DETECTED CIRCLE:
    cv::Mat colorImage;

    cv::cvtColor(grayImage, colorImage, cv::COLOR_GRAY2BGR);
    for (const auto &circle : circles)
    {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(colorImage, center, radius, cv::Scalar(0, 0, 255), 2); // draw red circle on gray image
    }

    // //testing image output:
    imshow(name, colorImage);
    int n = waitKey(0); // Wait for a keystroke in the window
}

void ClockDetection::drawDetectCircles(const vector<Vec3f> &circles, const cv::Mat& Image){
    for (const auto& circle : circles) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(Image, center, radius, cv::Scalar(0, 0, 255), 2); // draw red circle on gray image
    }
}

void ClockDetection::edgeDetection(const cv::Mat& grayImage, cv::Mat& detectedEdges, int lowThreshold, int highThreshold, int kernelSize, bool L2gradient){
    Canny(grayImage, detectedEdges, lowThreshold, highThreshold, kernelSize, L2gradient);
}

void ClockDetection::drawDetectedStandardLineCopy(string name, vector<Vec2f>& lines, const cv::Mat& grayImage){
    cv::Mat colorImage;

    cv::cvtColor(grayImage, colorImage, cv::COLOR_GRAY2BGR);
    for( size_t i = 0; i < lines.size(); i++ ){
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line(colorImage, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }

     // //testing image output:
    imshow(name, colorImage);
    int n = waitKey(0); // Wait for a keystroke in the window

}

void ClockDetection::drawDetectedStandardLine(vector<Vec2f> &lines, const cv::Mat& Image){
    for( size_t i = 0; i < lines.size(); i++ ){
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line(Image, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
}

void ClockDetection::drawDetectedProbabilisticLineCopy(string name, vector<Vec4i>& linesP, const cv::Mat& grayImage){
    cv::Mat colorImage;

    cv::cvtColor(grayImage, colorImage, cv::COLOR_GRAY2BGR);
    for (size_t i = 0; i < linesP.size(); i++) {
        Vec4i l = linesP[i];
        line(colorImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }

    // testing image output:
    imshow(name, colorImage);
    int n = waitKey(0); // Wait for a keystroke in the window

}

void ClockDetection::drawDetectedProbabilisticLine(vector<Vec4i> &linesP, const cv::Mat& Image){
    for (size_t i = 0; i < linesP.size(); i++) {
        Vec4i l = linesP[i];
        line(Image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }
}

vector<Vec4i> ClockDetection::filterLinesCloseToCenter(const vector<Vec4i>& lines, const Point& center,  int distanceThreshold){
    vector<Vec4i> filteredLines;

    // Iterate through each line and check if either endpoint is close to the center
    for (const auto& line : lines) {
        // endpoints 
        Point pt1(line[0], line[1]); 
        Point pt2(line[2], line[3]);

        // Calculate distance from each endpoint to the center
        double dist1 = norm(pt1 - center);
        double dist2 = norm(pt2 - center);

        // If either endpoint is close to the center, keep the line
        if (dist1 < distanceThreshold || dist2 < distanceThreshold) {
            filteredLines.push_back(line);
        }
    }

    return filteredLines;
}


std::tuple<Point2f, float, float> ClockDetection::detectEllipse(const cv::Mat &grayImage)
{
    // Detect edges using Canny edge detector
    Mat edges;
    // params: image, edges array, threshold1, threshold2
    Canny(grayImage, edges, 50, 150);

    // Find contours
    vector<vector<Point>> contours;
    // params: image, output contours, mode, method
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // returns a modified image

    // Filter contours based on area or other criteria
    vector<Point> largestContour;
    double maxArea = 0;
    for (const auto &contour : contours)
    {
        double area = contourArea(contour); // Computes Area of the contours
        // Finds max area
        if (area > maxArea)
        {
            maxArea = area;
            largestContour = contour;
        }
    }

    // Approximate contour to reduce the number of points
    vector<Point> approx;
    double epsilon = 0.01 * arcLength(largestContour, true);
    // params: array of curves, output array, epsilon, bool closed or not
    approxPolyDP(largestContour, approx, epsilon, true);

    // Fits an ellipse around a set of 2D points.
    // params: input array of points
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
    return std::make_tuple(center, radiusX, radiusY);
}
