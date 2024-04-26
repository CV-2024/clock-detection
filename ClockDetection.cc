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


void ClockDetection::drawDetectedProbabilisticLine(vector<Vec4i> &linesP, const cv::Mat& Image){
    for (size_t i = 0; i < linesP.size(); i++) {
        Vec4i l = linesP[i];
        line(Image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }
}

