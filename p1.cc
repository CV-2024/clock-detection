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
    // string image_path = "data_analog_clocks/real_staright_images/fake_hands/1-00/10.jpg";
    // string image_path = "data_analog_clocks/real_staright_images/real_hands/no_background/0_3_07.pgm";
    // string image_path = "data_analog_clocks/real_staright_images/real_hands/no_background/1_0_10.jpg";
    ClockDetection clockDetector(image_path);

    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    // Step 2: Convert the input image to grayscale
    Mat grayImage = clockDetector.convertToGray(img);
    
    // int maxRadius = grayImage.cols;
    int maxRadius = 200;
    // int minDist = grayImage.cols;
    int minDist = grayImage.rows / 8;
    int radius = 10;
    int param1 = 100; 
    int param2 = 30;
    int dp = 1; // the steps for resolution

    while (true){
        // Step 3: Detect circles with Hough Circle 
        vector<Vec3f> circles = clockDetector.detectCircles(grayImage, dp, param1, param2, minDist, radius, maxRadius);
        if (circles.size() > 0){
            cout << "Circles detected!" << endl;
            // Step 3a: Draw circles on the copy image 
            clockDetector.drawDetectCirclesCopy(circles, grayImage);
            clockDetector.drawDetectCircles(circles, img);

            // Print out center coordinates detected for image input
            for (const auto& circle : circles) {
                float x = circle[0];
                float y = circle[1];
                float radius = circle[2];
                std::cout << "Center: (" << x << ", " << y << "), Radius: " << radius << std::endl;
            }
            
            // other steps ...
            // Break the loop if both circles and lines are detected
            break;
        }
        else{
            // Step 3b: Increase radius and go back to step 2 (no cicle dectected)            
            radius++;
        }
    }
    // Display the original image with detected circles
    cout << "Breaks" << endl;
    imshow("Detected Circles", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    return 0;
}
