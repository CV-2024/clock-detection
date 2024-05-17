#include <iostream>
#include <opencv2/opencv.hpp>
#include "ClockDetection.h"

using namespace std;
using namespace cv;

int main()
{
    // Ellipse Detection -- second test image 0_5_04.jpeg
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
    Mat edges;
    std::tuple<Point2f, float, float> directions = clockDetector.detectEllipse(grayImage, edges);

    /* Agruments for  Probabilistic Hough Line Transform to detect lines*/
    int rho = 1;
    double theta = CV_PI / 180;
    int threshold = 50;
    int minLineLength = 50;
    int maxLineGap = 10;
    // Step 5: Use Probabilistic Hough Line Transform to detect lines
    vector<Vec4i> linesP;
    clockDetector.houghLinesP(edges, linesP, rho, theta, threshold, minLineLength, maxLineGap);

    if (linesP.size() >= 2)
    {
        // Step: 6: Check if lines are close to the center
        Point center(get<0>(directions).x, get<0>(directions).y);
        vector<Vec4i> filteredLines = clockDetector.filterLinesCloseToCenter(linesP, center, 20);

        if (filteredLines.size() >= 2)
        {
            // Step 7: pick the two lines that are close togehter
            Vec4i hourHand;
            Vec4i minHand;
            vector<Vec4i> hands;
            double minLength = 1000;
            double maxLength = 0;
            // hour hand
            for (auto &line : filteredLines)
            {
                // endpoints
                Point pt1(line[0], line[1]);
                Point pt2(line[2], line[3]);
                // length (distance formula)
                double length = sqrt(pow((line[0] - line[2]), 2) + pow((line[1] - line[3]), 2));
                if (length < minLength)
                {
                    minLength = length;
                    hourHand = line;
                }
            }
            // min hand
            for (auto &line : filteredLines)
            {
                // endpoints
                Point pt1(line[0], line[1]);
                Point pt2(line[2], line[3]);
                // length (distance formula)
                double length = sqrt(pow((line[0] - line[2]), 2) + pow((line[1] - line[3]), 2));
                if (length > maxLength)
                {
                    maxLength = length;
                    minHand = line;
                }
            }

            // hour hand
            line(img, Point(hourHand[0], hourHand[1]), Point(hourHand[2], hourHand[3]), Scalar(0, 255, 0), 3, LINE_AA);
            line(img, Point(minHand[0], minHand[1]), Point(minHand[2], minHand[3]), Scalar(0, 255, 0), 3, LINE_AA);

            imshow("Hour and Min Hands", img);
            waitKey(0); // Wait for a keystroke in the window
            hands.push_back(hourHand);
            hands.push_back(minHand);
        }
    }

    return 0;
}