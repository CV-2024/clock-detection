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

void ClockDetection::drawDetectCirclesCopy(const vector<Vec3f>& circles, const cv::Mat& grayImage){
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

void ClockDetection::calculateTime(const vector<Vec3f>& circles, const vector<Vec4i>& linesP) {
    if (circles.empty() || linesP.empty()) {
        cout << "Error: No circle or line detected!" << endl;
        return;
    }

    // Extract circle and line information
    Vec3f circle = circles[0]; 
    Point center(circle[0], circle[1]);
    Vec4i line1 = linesP[0];
    Vec4i line2 = linesP[1];

    // Identify which line corresponds to the hour hand and which one corresponds to the minute hand
    Vec4i hourHand, minuteHand;
    // norm: : Calculates the Euclidean distance between the start and end points
    if (norm(Point(line1[0], line1[1]) - Point(line1[2], line1[3])) < 
        norm(Point(line2[0], line2[1]) - Point(line2[2], line2[3]))) {
        hourHand = line1;
        minuteHand = line2;
    } else {
        hourHand = line2;
        minuteHand = line1;
    }

    // get all the points for the hour and miunte hands
    Point hourStart;
    Point hourEnd;
    
    if(norm(Point(hourHand[2], hourHand[3]) - center) > norm(Point(hourHand[0], hourHand[1]) - center)){
        hourEnd = Point(hourHand[2], hourHand[3]);
        hourStart = Point(hourHand[0], hourHand[1]);
    }
    else{
        hourEnd = Point(hourHand[0], hourHand[1]);
        hourStart = Point(hourHand[2], hourHand[3]);
    }
    // cout << "(1) Distance hourStart to center: " << norm(center - hourStart)<< endl;
    // cout << "(2) Distance hourStart to center: " << norm(hourStart - center)<< endl;

    // cout << "(1) Distance hourEnd to center: " << norm(center - hourEnd)<< endl;
    // cout << "(2) Distance hourEnd to center: " << norm(hourEnd - center)<< endl;

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
    // double hourSteps = angleInDegrees / 6.0; 
    // cout << "hourSteps: " << hourSteps << endl;
    double hourSteps;
    cout << "hourEnd.x: " << hourEnd.x << endl;
    cout << "center.x: " << center.x << endl;

    if (hourEnd.x > center.x) {
        // hourSteps = hourSteps / 30.0; 
        hourSteps = 6.0 * angleInDegrees / 180.0;
        //mins = 30 * angleInDegrees / 180.0
        cout << "Case 1: " << endl;
    } else {
        hourSteps = 12 - (6.0 * angleInDegrees / 180.0);
        // mins =  60 - 30 * angleInDegrees / 180.0
        cout << "Case 2: " << endl;
    }
    cout << "After Righ or left side hourSteps: " << hourSteps << endl;


    // Print out the steps for the hour hand and the minute hand
    cout << "Hour Hand Steps: " << hourSteps << endl;

    // again for the minute hand
    
    double side1_minute = norm(center - topPoint);
    double side2_minute = norm(center - minuteEnd);
    double side3_minute = norm(minuteEnd - topPoint);

    double cosine_minute = (pow(side2_minute, 2) + pow(side1_minute, 2) - pow(side3_minute, 2)) / (2 * side2_minute * side1_minute);
    double angleInRadians_minute = acos(cosine_minute);
    double angleInDegrees_minute = angleInRadians_minute * 180.0 / CV_PI;

    // double minuteSteps = angleInDegrees_minute / 30.0;
    // cout << "minuteSteps: " << minuteSteps / 30.0 << endl;
    double minuteSteps;

    if (minuteEnd.x > center.x) {
        // minuteSteps = angleInDegrees_minute / 6.0;
        minuteSteps = 30 * angleInDegrees_minute / 180.0;
    } else {
        // minuteSteps = (360.0 - angleInDegrees_minute) / 6.0;
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

    cout << "Time: " << hour << ":" << minute << endl;
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



