#include <iostream>
#include <opencv2/opencv.hpp>
#include "ClockDetection.h"

using namespace std;
using namespace cv;

int main()
{
    /*
    //Ellipse Detection --
    string image_path = "data_analog_clocks/real_angled_clocks/14222685283_573341bc36_z.jpeg";
    ClockDetection clockDetector(image_path);

    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    // Convert the input image to grayscale
    Mat grayImage = clockDetector.convertToGray(img);

    clockDetector.detectEllipse(grayImage);

    return 0;
    */
    string image_path = "data_analog_clocks/samples/1.jpg";
    ClockDetection clockDetector(image_path);

    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    // Convert the input image to grayscale
    Mat grayImage = clockDetector.convertToGray(img);

    // Display the grayscale image
    imshow("Gray Image", grayImage);
    int k = waitKey(0); // Wait for a keystroke in the window
    return 0;
}