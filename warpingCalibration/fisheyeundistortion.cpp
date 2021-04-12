#include "ui_mainwindow.h"
#include <opencv4/opencv2/stitching.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/calib3d/calib3d.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/highgui/highgui_c.h>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/core/utility.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/videoio.hpp>
using namespace std;
using namespace cv;

void fisheyeUndistortion(Mat& distortedIMG, Mat& undistortedIMG, Mat& map1, Mat& map2)
{


    //undistort(distortedIMG, undistortedIMG, cameraMatrix, distCoeffs);
    //fisheye::undistortImage(distortedIMG, undistortedIMG, cameraMatrix, distCoeffs);


//    Mat gray;
//    cvtColor(distortedIMG,gray,COLOR_BGR2GRAY);

//    remap(gray, undistortedIMG, map1, map2, INTER_LINEAR, BORDER_CONSTANT);
    remap(distortedIMG, undistortedIMG, map1, map2, INTER_LINEAR, BORDER_CONSTANT);


    namedWindow("Distorted", CV_WINDOW_KEEPRATIO);  //create window
    resizeWindow("Distorted", 500,500);             //set window size
    imshow("Distorted",distortedIMG);

    namedWindow("Undistorted", CV_WINDOW_KEEPRATIO);
    resizeWindow("Undistorted", 500,500);
    imshow("Undistorted",undistortedIMG);

    //Save image
    //string filename = "/home/ccs/Pictures/undistorted/undistorted1Img.png";
    //imwrite(filename,undistortedIMG);


    waitKey(0);

}
