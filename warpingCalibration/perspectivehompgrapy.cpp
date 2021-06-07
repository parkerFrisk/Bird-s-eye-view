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
void perspectiveHomograpy(Mat& undistortedIMG, int CHECKERBOARD[], Mat& H)
{
    /*** Get perspective transform (Is done for once per camera for the undistorted floored chessboard image)***/
    bool success=0;
    vector<Point2f> corner_pts;


    success = findChessboardCorners(undistortedIMG, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts);//, CALIB_RATIONAL_MODEL + CALIB_USE_INTRINSIC_GUESS + CALIB_FIX_PRINCIPAL_POINT);
    if(success)
    {
        qDebug("Success, Chessboard for bird's-eye view found");

        vector<Point2f> hImgP(4), hObjP(4);

        //Choose four corners on the chessboard

        hImgP[0] = corner_pts[0];
        hImgP[1] = corner_pts[CHECKERBOARD[0]-1];
        hImgP[2] = corner_pts[(CHECKERBOARD[1]-1)*CHECKERBOARD[0]];
        hImgP[3] = corner_pts[(CHECKERBOARD[1]-1)*CHECKERBOARD[0] + CHECKERBOARD[0]-1];

        int squareSize = 10; //Scales the chessboard

        hObjP[0].x = 0;
        hObjP[0].y = 0;

        hObjP[1].x = (CHECKERBOARD[0]-1) * squareSize;
        hObjP[1].y = 0;

        hObjP[2].x = 0;
        hObjP[2].y = (CHECKERBOARD[1] -1)*squareSize;

        hObjP[3].x = (CHECKERBOARD[0] -1)*squareSize;
        hObjP[3].y = (CHECKERBOARD[1] -1)*squareSize;



        // Calculate homography matrix (Is only done once as a part of calibraion)
        H = getPerspectiveTransform(hObjP, hImgP);
        H.at<double>(2,2)=1;



    }
    else{
        qDebug("Fail, Chessboard for bird's-eye view not found");
    }

}
