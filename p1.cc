// p1 - Hough Circle
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ClockDetection.h"

using namespace std;
using namespace cv;

int main()
{
    // Step 1: input image
    string image_path = "data_analog_clocks/samples/1.jpg";
    ClockDetection clockDetector(image_path);

    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    // Step 2: Convert the input image to grayscale
    Mat grayImage = clockDetector.convertToGray(img);
    
    int maxRadius = grayImage.cols;
    int minDist = 50;
    int radius = 50;
    int param1 = 45; 
    int param2 = 45;
    int dp = 1; // the steps
    while (true){
        // Step 3: Detect circles with Hough Circle 
        vector<Vec3f> circles = clockDetector.detectCircles(grayImage, dp, param1, param2, minDist, radius, maxRadius);
        if (circles.size() > 0){
            cout << "Circles detected!" << endl;
            // Step 3a: Draw circles on the original image
            // Note: created a Fucntion to do this!!!!
            for (size_t i = 0; i < circles.size(); i++)
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                circle(grayImage, center, radius, Scalar(0, 255, 0), 2);
            }

            // other steps ...

            // Break the loop if both circles and lines are detected
            break;
        }
        else{
            // Step 3b: Increase radius and go back to step 2 (no cicle dectected)
            // Step 6b: Increase radius and go back to step 2 (min of 2 lines not dectected)
            radius++;
        }
    }
    // Display the original image with detected circles
    imshow("Detected Circles", grayImage);
    int k = waitKey(0); // Wait for a keystroke in the window
    return 0;
}

// download: brew install opencv (for Mac)
// run:
// g++ -std=c++11 -I/usr/local/Cellar/opencv/4.9.0_7/include -I/usr/local/Cellar/opencv/4.9.0_7/include/opencv4 p1.cc ClockDetection.cc -o p1 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
// ./p1
