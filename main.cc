#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    string image_path = "hough_complex_1.pgm";
    Mat img = imread(image_path, IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    return 0;
}

// download: brew install opencv (for Mac)
// run:
// g++ -std=c++11 -I/usr/local/Cellar/opencv/4.9.0_7/include -I/usr/local/Cellar/opencv/4.9.0_7/include/opencv4 main.cc -o main -lopencv_core -lopencv_imgcodecs -lopencv_highgui
// ./main
