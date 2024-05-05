// p1 - Hough Circle
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ClockDetection.h"
#include <cmath>

using namespace std;
using namespace cv;

bool areLinesParallel(Vec4i line1, Vec4i line2)
{
    // y2-y1 / x2 - x1
    double slope1 = static_cast<double>(line1[3] - line1[1]) / (line1[2] - line1[0]);
    double slope2 = static_cast<double>(line2[3] - line2[1]) / (line2[2] - line2[0]);
    // Now find y-int using y = mx + b -> We know m and have a point [0] and [1] -> b = y - mx
    double line1Yintercept = static_cast<double>(line1[1] - slope1 * line1[0]);
    double line2Yintercept = static_cast<double>(line2[1] - slope2 * line2[0]);
    // If the slopes are the same and the y-intercepts are different, the lines are parallel
    return (slope1 == slope2 && line1Yintercept != line2Yintercept);
}

double distanceOfPoints(Vec4i line1, Vec4i line2)
{
    // find distance using distance formula sqrt((x2-x1)^2...)
    return static_cast<double>(std::sqrt(std::pow((line2[0] - line1[0]), 2) + std::pow((line2[1] - line1[1]), 2)));
}

int main()
{
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

    // result cirle and lines:
    vector<Vec3f> circlesResult;
    vector<Vec4i> linesPResult;

    /*Agruments for Hough Circle Function*/
    // int maxRadius = grayImage.cols;
    int maxRadius = 200;
    // int minDist = grayImage.cols;
    int minDist = grayImage.rows / 8;
    int radius = 10;
    int param1 = 100;
    int param2 = 30;
    int dp = 1; // the steps for resolution

    /*Agruments for Canyy edge detection Function*/
    int lowThreshold = 50;
    int highThreshold = 200;
    int kernelSize = 3;
    bool L2gradient = true;

    /*Parameters to filter if Close to teh center*/
    int distanceThreshold = 20;

    while (true)
    {
        // Step 3: Detect circles with Hough Circle
        vector<Vec3f> circles = clockDetector.detectCircles(grayImage, dp, param1, param2, minDist, radius, maxRadius);
        if (circles.size() > 0)
        {
            // If circle is found
            cout << "Circles detected!" << endl;
            // Step 3a: Draw circles on the copy image
            // clockDetector.drawDetectCirclesCopy(circles, grayImage);
            // clockDetector.drawDetectCircles(circles, img);

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
            HoughLinesP(edges, linesP, 1, CV_PI / 180, 50, 50, 10);
            if (linesP.size() >= 2)
            {
                cout << "linesP.size(): " << linesP.size() << endl;
                cout << "MIN 2 lines detected!" << endl;
                // clockDetector.drawDetectedProbabilisticLineCopy("minTwo?", linesP, grayImage);
                // clockDetector.drawDetectedProbabilisticLine(linesP, img);
                //  Step: 6: Check if lines are close to the center
                Point center(circles[0][0], circles[0][1]); // should be only one circle correct, if more than one this doesn't work!
                /*
                In short, each line is a Vec4i, the first two elements are the
                line's start point (x1,y1) and last two are the line's end point(x2,y2)
                */
                vector<Vec4i> filteredLines = clockDetector.filterLinesCloseToCenter(linesP, center, 20);
                // clockDetector.drawDetectedProbabilisticLineCopy("filteredLines", filteredLines, grayImage);
                if (filteredLines.size() >= 2)
                {

                    for (size_t i = 0; i < filteredLines.size(); ++i)
                    {
                        for (size_t j = 0; j < filteredLines.size(); ++j)
                        {
                            if (j == i)
                            {
                                continue;
                            }
                            if (areLinesParallel(filteredLines[i], filteredLines[j]))
                            {
                                // Lines i and j are parallel
                                // Do something here, like marking them or printing their indices
                                cout << "Lines " << i << " and " << j << " are parallel." << endl;
                                const double distance = distanceOfPoints(filteredLines[i], filteredLines[j]);
                                // check and see if distance is within a threshold if so then pop a line
                            }
                        }
                    }
                    std::cout << "Nothing found";
                    // Step 7: pick the two lines that are close togehter
                    // ...
                    // Step 8: Saved that circle and line into resuls
                    circlesResult = circles;
                    linesPResult = filteredLines;
                    // Break the loop if both circles and lines are detected
                    break;
                }
                else
                {
                    cout << "MIN 2 lines that are close to the center not detected!" << endl;
                    radius++;
                }
            }
            else
            {
                // Step 5b: Increase radius and go back to step 2 (min of 2 lines not dectected)
                cout << "MIN 2 lines NOT detected!" << endl;
                radius++;
                // continue;
            }

            // Print out center coordinates detected for image input
            for (const auto &circle : circles)
            {
                float x = circle[0];
                float y = circle[1];
                float radius = circle[2];
                std::cout << "Center: (" << x << ", " << y << "), Radius: " << radius << std::endl;
            }

            // other steps ...
            // Break the loop if both circles and lines are detected
            break;
        }
        else
        {
            // Step 3b: Increase radius and go back to step 2 (no cicle dectected)
            radius++;
        }
    }

    cout << "Breaks" << endl;
    // step 9: Math using result circle and line( vector<Vec3f> circlesResult and vector<Vec4i> linesPResult)
    // clockDetector.calculateTime(circlesResult, linesPResult);
    // Step 10: Display the original image with detected circles
    imshow("Detected Circles", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    return 0;
}
