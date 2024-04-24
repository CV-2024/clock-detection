#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "ClockDetection.h"

using namespace std;
using namespace cv;

int main()
{
    string image_path = "data_analog_clocks/samples/1.jpg";
    ClockDetection clockDetector(image_path);

    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    if(img.channels() == 3) {
        cout << "Image is color" << endl;
    } else {
        cout << "Image is grayscale" << endl;
    }

    // Convert the input image to grayscale
    Mat grayImage = clockDetector.convertToGray(img);

    // Display the grayscale image
    imshow("Gray Image", grayImage);
    int k = waitKey(0); // Wait for a keystroke in the window
    
    //Establish new copy of grayscale image to detect circles in:
    Mat circleImg = imread(image_path, IMREAD_COLOR);

    // //testing image output:
    // imshow("Color Copy", circleImg);
    // int l = waitKey(0); // Wait for a keystroke in the window

    // Convert the input image to grayscale
    Mat grayCircleImg = clockDetector.convertToGray(circleImg);

    // //testing image output:
    // imshow("Color Copy", grayCircleImg);
    // int m = waitKey(0); // Wait for a keystroke in the window

    //Detect Circles
    std::vector<cv::Vec3f> detectedCircles = clockDetector.detectCircles(grayCircleImg);

    // Print out center coordinates detected for image input
    for (const auto& circle : detectedCircles) {
        float x = circle[0];
        float y = circle[1];
        float radius = circle[2];
        std::cout << "Center: (" << x << ", " << y << "), Radius: " << radius << std::endl;
    }

    return 0;
}


// download: brew install opencv (for Mac)
// run:
// g++ -std=c++11 -I/usr/local/Cellar/opencv/4.9.0_7/include -I/usr/local/Cellar/opencv/4.9.0_7/include/opencv4 main.cc ClockDetection.cc -o main -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
// ./main