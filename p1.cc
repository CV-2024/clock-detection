// p1 - Hough Circle
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ClockDetection.h"

using namespace std;
using namespace cv;

int main(){
    // Step 1: input image
    string image_path = "data_analog_clocks/samples/1.jpg";
    // string image_path = "data_analog_clocks/real_staright_images/fake_hands/1-00/10.jpg"; // with background 
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
    
    /*Agruments for Hough Circle Function*/

    int maxRadius = grayImage.cols;
    // int maxRadius = 200;
    int minDist = grayImage.cols;
    // int minDist = grayImage.rows / 8;
    int radius = 10;
    int param1 = 100; 
    int param2 = 30;
    int dp = 1; // the steps for resolution

    /*Agruments for Canyy edge detection Function*/
    int lowThreshold = 50;
    int highThreshold = 200; 
    int kernelSize = 3; 
    bool L2gradient = true;

    while (true){
        // Step 3: Detect circles with Hough Circle 
        vector<Vec3f> circles = clockDetector.detectCircles(grayImage, dp, param1, param2, minDist, radius, maxRadius);
        if (circles.size() > 0){
            cout << "Circles detected!" << endl;
            // Step 3a: Draw circles on the copy image 
            clockDetector.drawDetectCirclesCopy(circles, grayImage);
            clockDetector.drawDetectCircles(circles, img);

        
            // step 4: Use CANNY for line
            Mat edges;
            clockDetector.edgeDetection(grayImage, edges, lowThreshold, highThreshold, kernelSize, L2gradient);
            imshow("cannyOutput", edges);

            // // Step 5: Use Standard Hough Line Transform to detect lines 
            // vector<Vec2f> lines; 
            // HoughLines(edges, lines, 1, CV_PI/180, 50, 0, 0); 
            // Step 6: Check for a min of 2 lines
            // if (lines.size() >= 2){
            //     // Draw detected lines from Use Standard Hough Line on the original image
            //     cout << "lines.size(): " << lines.size() << endl;
            //     cout << "MIN 2 lines detected!" << endl;
            //     clockDetector.drawDetectedStandardLineCopy(lines, grayImage);
            //     clockDetector.drawDetectedStandardLine(lines, img);
            //     // Break the loop if both circles and lines are detected
            //     break;
            // }
            
            // Step 5: Use Probabilistic Hough Line Transform to detect lines 
            vector<Vec4i> linesP; 
            // Step 6: Draw detected lines from Use Probabilistic Hough Line on the original image
            HoughLinesP(edges, linesP, 1, CV_PI/180, 50, 50, 10 );
            if (linesP.size() >= 2){
                cout << "linesP.size(): " << linesP.size() << endl;
                cout << "MIN 2 lines detected!" << endl;
                    clockDetector.drawDetectedProbabilisticLineCopy(linesP, grayImage);
                    clockDetector.drawDetectedProbabilisticLine(linesP, img);
                // Break the loop if both circles and lines are detected
                break;
            }
            else{
                // Step 5b: Increase radius and go back to step 2 (min of 2 lines not dectected)
                cout << "MIN 2 lines NOT detected!" << endl;
                radius++;
                // continue;
            }

            // other steps ...

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
