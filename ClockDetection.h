#ifndef CLOCK_DETECTION_H
#define CLOCK_DETECTION_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ClockDetection {
    private:
        string image_path;
        // vector<Vec3f> circles;
        // Mat edges;
    public:
        // Default constructor
        ClockDetection(const string& imagePath);
        /*
        @breif convert colored image to greyscale
        @param const cv::Mat& - color image 
        */
        cv::Mat convertToGray(const cv::Mat& inputImage);


        /*
        @brief Detect circles in a grayscale image using Hough Circle Transformation
        @param grayImage - Grayscale image
        @param dp - a ingter that is an inverse ratio. It essentially controls the resolution of the accumulator relative to the resolution of the input image. 
        @param minDist - a ingter that specifies the minimum distance between the centers of the detected circles.
        @param param1 - a ingter that is the higher threshold,for HOUGH_GRADIENT. The lower threshold is twice smaller.
        @param param2 - a ingter that is the accumulator threshold for the circle centers at the detection stage.
        @param minRadius - Minimum radius to start the search
        @param maxRadius - Max radius to start the search
        @return Detected circles (center coordinates and radius)
        */
       vector<Vec3f> detectCircles(const cv::Mat& grayImage, int dp, int param1, int param2, int minDist, int minRadius, int maxRadius);

       /*
        @brief Draw the Circle on the passed image directly
        @param Image - input image to be drawn on 
        @param circles - vector of Detected circles(center coordinates and radius)
       */
       void drawDetectCircles(const vector<Vec3f> &circles, const cv::Mat& Image);
       /*
        @brief Draw the Circle on the copy of the passed image and display it.
        @param grayImage - Grayscale image
        @param circles - vector of Detected circles(center coordinates and radius)
       */
       void drawDetectCirclesCopy(string name, const vector<Vec3f> &circles, const cv::Mat& grayImage);

       /*
       @brief apply edge detection using CANNY to an greyScale image.
       @param grayImage - Grayscale image
       @param detectedEdges - Output of the detector
       @param lowThreshold - integer value that is the low thereshold 
       @param highThreshold - integer value that is the low thereshold 
       @param kernelSize -  integer value that is the size of the Sobel kernel to be used internally
       @param L2gradient - boolean  that indicates whether to use 𝐿2 norm for gradient calculation. If set to true, the function will use the 𝐿2 norm (square root of the sum of squares of gradient magnitudes) to compute the gradient magnitude. If set to false, it will use the 𝐿1 norm (sum of absolute values of gradient magnitudes). Typically, setting this parameter to true produces better results but is slower.
       */
       void edgeDetection(const cv::Mat& grayImage, cv::Mat& detectedEdges, int lowThreshold, int highThreshold, int kernelSize, bool L2gradient);

        /*
        @brief apply  Probabilistic Hough Line Transform to detect lines 
        @param edges - The input binary image where edges are detected. 
        @param linesP - the output vector if decteded lines 
        @param rho - the distance resolution in pixels of the Hough accumulator array or the steps
        @param theta -  It is the angle resolution in radians of the Hough accumulator array or the step
        @param threshold - local maxima or the minimum number of intersections in the Hough space to detect a line.
        @param minLineLength - min length of the detected line 
        @param maxLineGap -  max the gap between line segments
        */
        void houghLinesP(const Mat& edges,  vector<Vec4i>& linesP, int rho, double theta, int threshold, int minLineLength, int maxLineGap);

        /*
        @brief Draw the Standard line on the passed image directly
        @param Image - input image to be drawn on 
        @param lines - vector of Detected line(with endpoints)
        */
       void drawDetectedStandardLine(vector<Vec2f> &lines, const cv::Mat& Image);
       
       /*
        @brief Draw the Standard Line on the copy of the passed image and display it.
        @param grayImage - Grayscale image
        @param lines - vector of Detected line(with endpoints)
       */
       void drawDetectedStandardLineCopy(vector<Vec2f> &lines, const cv::Mat& grayImage);


        /*
        @brief Draw the Probabilistic line on the passed image directly
        @param Image - input image to be drawn on 
        @param lines - vector of Detected line(with endpoints)
        */
       void drawDetectedProbabilisticLine(vector<Vec4i> &linesP, const cv::Mat& Image);
       
       /*
        @brief Draw the Probabilistic Line on the copy of the passed image and display it.
        @param grayImage - Grayscale image
        @param lines - vector of Detected line(with endpoints)
       */
       void drawDetectedProbabilisticLineCopy(string name, vector<Vec4i> &linesP, const cv::Mat& grayImage);

        /*
        @brief the trigeometry used to compute the time 
        @param circles - vector of Detected circles(center coordinates and radius)
        @param lines - vector of Detected line
       */
       void calculateTime(const vector<Vec3f>& circles, const vector<Vec4i>& linesP);

        /*
        @brief Filters out the lines that are not close to the center 
        @param lines - vector of Detected line(with endpoints)
        @param center - the center Point of the detected circle 
        @param distanceThreshold - integer distance theshold from the center 
        */
       vector<Vec4i> filterLinesCloseToCenter(const vector<Vec4i>& lines, const Point& center, int distanceThreshold);

       /*
    @brief Detect ellipse in a grayscale image
    @param const cv::Mat& - grayscale image
    @return vector<cv::Vec3f> - detected ellipse (center coordinates and radius)
    */
    std::tuple<Point2f, float, float> detectEllipse(const cv::Mat &grayImage, cv::Mat &edgesDetected);




};

#endif /* CLOCK_DETECTION_H */