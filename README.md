# Clock Detection 

How to Run Requirements:
- OpenCV library installed.
- C++ compiler.
- Compilation:
    - Ensure OpenCV is properly linked in the Makefile.
    - Run make command to compile the program: 
        - make all

Program_1 p1:
    General Descrition: 
        Gemeatric approah to read a the time of an Analogy clock. Used this paper was a starting point: https://www.diva-portal.org/smash/get/diva2:1668098/FULLTEXT02
    
    Dataset: Test / works best on the  "data_analog_clocks/synthetic_straight_images/color" images 

    Important Arguments for built-in openCV Functions:
        Agruments for Hough Circle Function:
            dp - a ingter that is an inverse ratio. It essentially controls the resolution of the accumulator relative to the resolution of the input image. 
            minDist - a ingter that specifies the minimum distance between the centers of the detected circles.
            param1 - a ingter that is the higher threshold,for HOUGH_GRADIENT. The lower threshold is twice smaller.
            param2 - a ingter that is the accumulator threshold for the circle centers at the detection stage.
            minRadius - Minimum radius to start the search
            maxRadius - Max radius to start the search

            dp = 1
            minDist = image number of columns 
            param1 = 100
            param2 = 30
            minRadius = 10
            maxRadius = image number of columns 

        Agruments for Canyy edge detection Function:
            lowThreshold - integer value that is the low thereshold 
            highThreshold - integer value that is the low thereshold 
            kernelSize -  integer value that is the size of the Sobel kernel to be used internally
            L2gradient - boolean  that indicates whether to use 𝐿2 norm for gradient calculation. If set to true, the function will use the 𝐿2 norm (square root of the sum of squares of gradient magnitudes) to compute the gradient magnitude. If set to false, it will use the 𝐿1 norm (sum of absolute values of gradient magnitudes). Typically, setting this parameter to true produces better results but is slower.

            lowThreshold = 50
            highThreshold = 200 
            kernelSize = 3
            L2gradient = false
        
        Agruments for  Probabilistic Hough Line Transform:
            rho - the distance resolution in pixels of the Hough accumulator array or the steps
            theta -  It is the angle resolution in radians of the Hough accumulator array or the step
            threshold - local maxima or the minimum number of intersections in the Hough space to detect a line.
            minLineLength - min length of the detected line 
            maxLineGap -  max the gap between line segments

            rho = 1
            theta = CV_PI/180
            threshold = 50
            minLineLength = 10
            maxLineGap = 50

    Limitation:
        - detected Border Lines - the line the trace the hand are what is detected and one of them is chosen to the hand we read, so the time is off my a 1 or 2 sometimes. 
        - Doesn't work will with Background, i.e., if something is behind the clock or if the shade of color between the hands and the back on the clock are similar a greyscale image will cause a problem. In that case feed a the colored image to the edge detected function.
        - Algorithm Sensitivity - there must be two hands and one circle feed to the math function. If its more ot the wrong lines are detected then the programs fails. 
        - Finding the Hand and Minute - the program is also sensitivity to the length of the line that are detected. IF the hour is no the shortest and the minute not the Lonest then it willl function. So the output of LineHough is very important
        - No second  - will not work if there is a seconds hand. 

Program_2 p2:
    General Descrition: 
        Contour based method of identifying angled clocks in images
    
    Dataset: Test / works best on the  "data_analog_clocks/real_angled_clocks/14222685283_573341bc36_z.jpeg" image

Important Arguments for built-in openCV Functions:
        Agruments for Canny edge detection Function:
            InputArray Image - Input image
            OutputArray Edges - Output edge image
            double Threshold1 - Low threshold
            double Threshold2 - High threshold

            Canny(grayImage, edges, 50, 150);
        
        Agruments for FindContours Function:
            InputOutputArray Image - Input image 
            OutputArrayOfArrays Contours - Vector of points storing the detected contours
            int Mode - Used RETR_EXTERNAL for extracting the outer region contours of the input image
            int Method - Used CHAIN_APPROX_SIMPLE to compress the contour

            findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Agruments for approxPolyDP Function:
            InputArray Curve - Input contour to be approximated
            OutputArray ApproxCurve - Outputs the approximated curves of the contour using the Douglas-Peucker algorithm
            double Epsilon - The approximation accuracy
            bool Closed - We used true so that the approximated curve is closed so all the vertices are connected

            approxPolyDP(largestContour, approx, epsilon, true);

        Agruments for fitEllipse Function:
            Points - A set of 2D points. This functions fits an ellipse around a set of 2D points and returns a rotated rectangle

            RotatedRect ellipse = fitEllipse(approx);

    Limitation:
        - Algorithm & Image Sensitivity - the location of the center of the identified clock may be off due to the contours generated by the openCV function and the fit function
