#include "ClockDetection.h"

ClockDetection::ClockDetection(const string& imagePath) : image_path(imagePath) {}

cv::Mat ClockDetection::convertToGray(const cv::Mat& inputImage) {
    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}

vector<Vec3f> ClockDetection::detectCircles(const cv::Mat& grayImage, int dp, int param1, int param2, int minDist, int minRadius, int maxRadius){
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
    for (const auto& circle : circles) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(colorImage, center, radius, cv::Scalar(0, 0, 255), 2); //draw red circle on gray image
    }

    // //testing image output:
    imshow(name, colorImage);
    int n = waitKey(0); // Wait for a keystroke in the window
}

void ClockDetection::drawDetectCircles(const vector<Vec3f> &circles, const cv::Mat& Image){
    for (const auto& circle : circles) {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(Image, center, radius, cv::Scalar(0, 0, 255), 2); //draw red circle on gray image
    }
}

void ClockDetection::edgeDetection(const cv::Mat& grayImage, cv::Mat& detectedEdges, int lowThreshold, int highThreshold, int kernelSize, bool L2gradient){
    
    Canny(grayImage, detectedEdges, lowThreshold, highThreshold, kernelSize, L2gradient);
}

void ClockDetection::houghLinesP(const Mat& edges,  vector<Vec4i>& linesP, int rho, double theta, int threshold,  int minLineLength, int maxLineGap){
    cv::HoughLinesP(edges, linesP, rho, theta, threshold, minLineLength, maxLineGap);
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

void ClockDetection::calculateTime(const vector<Vec3f>& circles, const vector<Vec4i>& linesP) {
    if (circles.empty() || linesP.empty()) {
        cout << "Error: No circle or line detected!" << endl;
        return;
    }

    // Extract circle and line information
    Vec3f circle = circles[0]; 
    Point center(circle[0], circle[1]);

    Vec4i hourHand  = linesP[0];
    Vec4i minuteHand = linesP[1];

    /* FIND THE HOUR AND MINUTE HANDS */
        // Vec4i line1 = linesP[0];
        // Vec4i line2 = linesP[1];
        // Identify which line corresponds to the hour hand and which one corresponds to the minute hand
        // Vec4i hourHand, minuteHand;
        // // norm: : Calculates the Euclidean distance between the start and end points
        // if (norm(Point(line1[0], line1[1]) - Point(line1[2], line1[3])) < 
        //     norm(Point(line2[0], line2[1]) - Point(line2[2], line2[3]))) {
        //     hourHand = line1;
        //     minuteHand = line2;
        // } else {
        //     hourHand = line2;
        //     minuteHand = line1;
        // }

    // Get all the points for the hour and miunte hands
    Point hourStart;
    Point hourEnd;
    
    // Determine which point is the start and end 
    if(norm(Point(hourHand[2], hourHand[3]) - center) > norm(Point(hourHand[0], hourHand[1]) - center)){
        hourEnd = Point(hourHand[2], hourHand[3]);
        hourStart = Point(hourHand[0], hourHand[1]);
    }
    else{
        hourEnd = Point(hourHand[0], hourHand[1]);
        hourStart = Point(hourHand[2], hourHand[3]);
    }
    Point minuteStart(minuteHand[0], minuteHand[1]);
    Point minuteEnd(minuteHand[2], minuteHand[3]);



    // the x-axis at the top of the image
    Point topPoint(center.x, 0);

    // Calculate the lengths of the sides of the triangle
    double side1 = norm(center - topPoint);
    // cout << "side1_hour: " << side1 << endl;
    double side2 = norm(center - hourEnd);
    // cout << "side2_hour: " << side2 << endl;
    double side3 = norm(hourEnd - topPoint);
    // cout << "side3_hour: " << side3 << endl;

    // Apply the law of cosines to calculate the cosine of the angle between center line and the watch hand line. 
    double cosine = (pow(side2, 2) + pow(side1, 2) - pow(side3, 2)) / (2 * side2 * side1);
    cout << "cosine: " << cosine << endl;


    // Compute the angle from the cosine value (in radians)
    double angleInRadians = acos(cosine);
    cout << "angleInRadians: " << angleInRadians << endl;



    // Convert the angle from radians to degrees
    double angleInDegrees = angleInRadians * 180.0 / CV_PI;
    cout << "angleInDegrees: " << angleInDegrees << endl;

    // Determine the number of steps for the hour hand and the minute hand
    double hourSteps;
    if (hourEnd.x > center.x) {
        hourSteps = 6.0 * angleInDegrees / 180.0;
    } else {
        hourSteps = 12 - (6.0 * angleInDegrees / 180.0);
    }


    // Print out the steps for the hour hand and the minute hand
    cout << "Hour Hand Steps: " << hourSteps << endl;

    // Again, but for the minute hand
    
    double side1_minute = norm(center - topPoint);
    double side2_minute = norm(center - minuteEnd);
    double side3_minute = norm(minuteEnd - topPoint);

    double cosine_minute = (pow(side2_minute, 2) + pow(side1_minute, 2) - pow(side3_minute, 2)) / (2 * side2_minute * side1_minute);
    double angleInRadians_minute = acos(cosine_minute);
    double angleInDegrees_minute = angleInRadians_minute * 180.0 / CV_PI;

    double minuteSteps;

    if (minuteEnd.x > center.x) {
        minuteSteps = 30 * angleInDegrees_minute / 180.0;
    } else {
        minuteSteps = 60 - (30 * angleInDegrees_minute / 180.0);
    }

    cout << "Minute Hand Steps: " << minuteSteps << endl;

    // Compute the time based on the calculated steps for the hour and minute hands
    int hour = static_cast<int>(hourSteps);
    int minute = static_cast<int>(minuteSteps);

    // Ensure within valid ranges
    cout << "HOUR: " << hour << endl;
    hour = (hour >= 0 && hour <= 12) ? hour : 0;
    minute = (minute >= 0 && minute <= 59) ? minute : 0;

    // if it single digit minute add a 0
    string minuteString = (minute < 10) ? "0" + to_string(minute) : to_string(minute);
    cout << "Time: " << hour << ":" << minuteString << endl;

}
