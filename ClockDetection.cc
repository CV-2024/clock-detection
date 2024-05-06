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

void ClockDetection::drawDetectCirclesCopy(const vector<Vec3f>& circles, const cv::Mat& grayImage){
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
    imshow("Draw Copy", colorImage);
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


void ClockDetection::drawDetectedStandardLineCopy(vector<Vec2f>& lines, const cv::Mat& grayImage){
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
    imshow("Draw Copy", colorImage);
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

void ClockDetection::drawDetectedProbabilisticLineCopy(vector<Vec4i>& linesP, const cv::Mat& grayImage){
    cv::Mat colorImage;

    cv::cvtColor(grayImage, colorImage, cv::COLOR_GRAY2BGR);
    for (size_t i = 0; i < linesP.size(); i++) {
        Vec4i l = linesP[i];
        line(colorImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }

    // testing image output:
    imshow("Draw Copy", colorImage);
    int n = waitKey(0); // Wait for a keystroke in the window

}

//DZ testing
void ClockDetection::calculateClockTime(const vector<Vec3f>& circles, const vector<Vec4i>& linesP) {

    // // Assuming the first circle detected represents the clock face
    // Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));

    // cout <<"centers: " << circles[0][0] << " and " << circles[0][1] << endl;

    // // Assuming the first two lines detected represent the hour and minute hands
    // Vec4i hourHand = linesP[0];
    // Vec4i minuteHand = linesP[1];
    // cout << "hourHand: " << hourHand << endl;
    // cout << "minuteHand: " << minuteHand << endl;

    // //GET ANGLE BETWEEN HANDS
    // // Calculate the vectors representing the hands
    // Point hourVec(hourHand[2] - hourHand[0], hourHand[3] - hourHand[1]);
    // Point minuteVec(minuteHand[2] - minuteHand[0], minuteHand[3] - minuteHand[1]);

    // // Calculate the dot product of the vectors
    // double dotProduct = hourVec.x * minuteVec.x + hourVec.y * minuteVec.y;

    // // Calculate the magnitudes of the vectors
    // double hourMagnitude = sqrt(hourVec.x * hourVec.x + hourVec.y * hourVec.y);
    // double minuteMagnitude = sqrt(minuteVec.x * minuteVec.x + minuteVec.y * minuteVec.y);

    // // Calculate the cosine of the angle between the vectors
    // double cosAngle = dotProduct / (hourMagnitude * minuteMagnitude);

    // // Calculate the angle in radians using the arccosine function
    // double angleRadians = acos(cosAngle);

    // // Convert the angle to degrees
    // double angleDegrees = angleRadians * 180.0 / CV_PI;
    // //END ANGLE BETWEEN HANDS

    // //Laws of Cosines:
    // double hourLength = sqrt(pow(hourHand[2] - hourHand[0], 2) + pow(hourHand[3] - hourHand[1], 2));
    // double minLength = sqrt(pow(minuteHand[2] - minuteHand[0], 2) + pow(minuteHand[3] - minuteHand[1], 2));
    // double hourSide = pow(hourLength, 2) + pow(minLength, 2) - (2*hourLength*minLength*cos(angleDegrees));

    // double radHour = acos(hourLength);
    // double radMin = acos(minLength);

    // double degHour = radHour * 180 / CV_PI;
    // double degMinute = radMin * 180 / CV_PI;

    // Calculate the length of sides of the triangle formed by the hands and the center point
    //double sideHour = sqrt(pow(hourHand[2] - hourHand[0], 2) + pow(hourHand[3] - hourHand[1], 2));
    //double sideMinute = sqrt(pow(minuteHand[2] - minuteHand[0], 2) + pow(minuteHand[3] - minuteHand[1], 2));
    //double sideRadius = sqrt(pow(center.x - hourHand[0], 2) + pow(center.y - hourHand[1], 2));

    // Calculate the cosine of the angles between the hands and the center line
    //double cosHour = (pow(sideRadius, 2) + pow(sideHour, 2) - pow(sideHour, 2)) / (2 * sideRadius * sideHour);
    //double cosMinute = (pow(sideRadius, 2) + pow(sideMinute, 2) - pow(sideMinute, 2)) / (2 * sideRadius * sideMinute);

    // Convert cosines to radians
    //double radHour = acos(cosHour);
    //double radMinute = acos(cosMinute);

    // Convert radians to degrees
    //double degHour = radHour * 180 / CV_PI;
    //double degMinute = radMinute * 180 / CV_PI;


    // Output the calculated time
    //cout << "The time indicated by the clock is: " << hours << ":" << setw(2) << setfill('0') << minutes << endl;

        // Assuming the first circle detected represents the clock face
    // Assuming the first circle detected represents the clock face
    // Assuming the first circle detected represents the clock face
    // Assuming the first circle detected represents the clock face
    Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));

    // Assuming the first two lines detected represent the hour and minute hands
    Vec4i hourHand = linesP[0];
    Vec4i minuteHand = linesP[1];

    // Calculate the angle between the hour hand and the vertical (12 o'clock) position
    double hourAngle = atan2(hourHand[1] - hourHand[3], hourHand[0] - hourHand[2]) * 180 / CV_PI;
    if (hourAngle < 0) {
        hourAngle += 360; // Adjust angle to be between 0 and 360 degrees
    }

    // Calculate the angle between the minute hand and the vertical (12 o'clock) position
    double minuteAngle = atan2(minuteHand[1] - minuteHand[3], minuteHand[0] - minuteHand[2]) * 180 / CV_PI;
    if (minuteAngle < 0) {
        minuteAngle += 360; // Adjust angle to be between 0 and 360 degrees
    }

    // Calculate the hour and minute from the angles
    int hours = static_cast<int>((hourAngle / 30) + 0.5); // Each hour is 30 degrees
    int minutes = static_cast<int>((minuteAngle / 6) + 0.5); // Each minute is 6 degrees

    // Adjust hours and minutes to be within 12-hour clock format
    hours %= 12;
    if (hours == 0) {
        hours = 12;
    }

    // Output the calculated time in 12-hour format
    cout << "The time is: " << hours << ":" << minutes << endl;
}
//END DZ TESTING

void ClockDetection::drawDetectedProbabilisticLine(vector<Vec4i> &linesP, const cv::Mat& Image){
    for (size_t i = 0; i < linesP.size(); i++) {
        Vec4i l = linesP[i];
        line(Image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }
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
