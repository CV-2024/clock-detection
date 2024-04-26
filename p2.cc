#include <iostream>
#include <opencv2/opencv.hpp>
#include "ClockDetection.h"

using namespace std;
using namespace cv;

int main()
{
    // Ellipse Detection -- second test image 0_5_04.jpeg
    // string image_path = "data_analog_clocks/real_angled_clocks/14222685283_573341bc36_z.jpeg";
    string image_path = "data_analog_clocks/real_angled_clocks/0_5_04.jpeg";

    ClockDetection clockDetector(image_path);

    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    // Convert the input image to grayscale
    Mat grayImage = clockDetector.convertToGray(img);

    std::tuple<Point2f, float, float> directions = clockDetector.detectEllipse(grayImage);

    return 0;
}