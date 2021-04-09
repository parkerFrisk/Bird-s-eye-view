#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include "opencv2/calib3d.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/highgui.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/imgproc.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

#include "opencv2/stitching.hpp"
#include "main.h"


using namespace std;
using namespace cv;
using std::cout;
using std::endl;


int main( int argc, char** argv )
{


    // Load images
    Mat image1 = imread("img1");
    Mat image2 = imread("img2");

    if( image1.empty() ) { cout << "Error loading src1" << endl; return -1; }
    if( image2.empty() ) { cout << "Error loading src2" << endl; return -1; }


    // Init chessboard size
    int board_w = 4; //8 /6 /4
    int board_h = 3; //6 /4 /3
    int CHECKERBOARD[2]{board_w,board_h};

    // Find chessboard
    vector<Point2f> cornersImg1, cornersImg2;
    bool successImg1= findChessboardCorners(image1, Size(CHECKERBOARD[0], CHECKERBOARD[1]), cornersImg1, CALIB_RATIONAL_MODEL + CALIB_USE_INTRINSIC_GUESS + CALIB_FIX_PRINCIPAL_POINT);
    bool successImg2 = findChessboardCorners(image2, Size(CHECKERBOARD[0], CHECKERBOARD[1]), cornersImg2, CALIB_RATIONAL_MODEL + CALIB_USE_INTRINSIC_GUESS + CALIB_FIX_PRINCIPAL_POINT);

    cout << "Success img1: " << successImg1 << endl;
    cout << "Success img2: " << successImg2 << endl;

}




