#include <iostream>
#include <opencv2/opencv.hpp>
#include "ClockDetection.h"

using namespace std;
using namespace cv;

int main()
{
    string image_path = "data_analog_clocks/samples/1.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    // Create an instance of ClockDetection
    ClockDetection clockDetector;

    // Convert the input image to grayscale
    Mat grayImage = clockDetector.convertToGray(img);

    // Display the grayscale image
    imshow("Gray Image", grayImage);
    int k = waitKey(0); // Wait for a keystroke in the window
    return 0;
}

// download: brew install opencv (for Mac)
// run:
// g++ -std=c++11 -I/usr/local/Cellar/opencv/4.9.0_7/include -I/usr/local/Cellar/opencv/4.9.0_7/include/opencv4 main.cc ClockDetection.cc -o main -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
// ./main
