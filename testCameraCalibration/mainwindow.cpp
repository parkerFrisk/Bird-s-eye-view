#include "mainwindow.h"
#include "ui_mainwindow.h"
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
//#include <stdio.h>
//#include <iostream>
//#include <vector>
//#include <chrono>

//So that opevCV functions can be used directly (without a cv:: or std everytime)
using namespace std;
using namespace cv;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

//https://learnopencv.com/camera-calibration-using-opencv/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    auto t1 = high_resolution_clock::now();

    int board_w = 8;
    int board_h = 6;
    int board_n = board_w * board_h;


    // Defining the dimensions of checkerboard
    int CHECKERBOARD[2]{board_w,board_h};

    // Creating vector to store vectors of 3D points for each checkerboard image
    vector<vector<Point3f> > objPoints;

    // Creating vector to store vectors of 2D points for ea ch checkerboard image
    vector<vector<Point2f> > imgPoints;


    // Defining the world coordinates for 3D points
    vector<Point3f> objp;
    for(int i{0}; i<CHECKERBOARD[1]; i++)
      {
        for(int j{0}; j<CHECKERBOARD[0]; j++)
          objp.push_back(Point3f(j,i,0));
      }


    // Extracting path of individual image stored in a given directory
    vector<String> images;
    // Path of the folder containing checkerboard images (OBS! key sensitive)
    string path = "/home/ccs/Pictures/*.jpg";
    qDebug("path ok ");
    glob(path, images);


    Mat frame, gray, birdImage;
    // vector to store the pixel coordinates of detected checker board corners
    vector<Point2f> corner_pts;
    bool success;


    // Looping over all the images in the directory
    for(int i{0}; i<images.size(); i++)
      {
        qDebug("For image %d", i+1);
        frame = imread(images[i]);

        if(!frame.data){
            qDebug("image %d broken", i+1);
        }
        else {
            qDebug("image %d cool", i+1);
        }

        //Creating gray scal cpy
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        namedWindow("Gray", CV_WINDOW_KEEPRATIO);
        resizeWindow("Gray", 500,500);
        imshow("Gray",gray);


        // Finding checker board corners
        success = findChessboardCorners(gray, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_FAST_CHECK + CALIB_CB_NORMALIZE_IMAGE);



        if(success)
        {
          qDebug("Success, Chessboard found %d", i+1);


          // refining pixel coordinates for given 2d points.
          cornerSubPix(gray,corner_pts,Size(11,11), Size(-1,-1),TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001 ));


          // Displaying the detected corner points on the checker board
          drawChessboardCorners(frame, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);




          /** Find homography
          vector<Point2f> hImgP(4), hObjP(4);

          //Choose four corners on the chessboard
          hImgP[0] = corner_pts[0];
          hImgP[1] = corner_pts[board_w-1];
          hImgP[2] = corner_pts[(board_h-1)*board_w];
          hImgP[3] = corner_pts[(board_h-1)*board_w + board_w-1];



          hObjP[0].x = 0;
          hObjP[0].y = 0;

          hObjP[1].x = board_w -1;
          hObjP[1].y = 0;

          hObjP[2].x = 0;
          hObjP[2].y = board_h -1;

          hObjP[3].x = board_w -1;
          hObjP[3].y = board_h -1;

          // Calculate homography matrix
          Mat H = getPerspectiveTransform(hObjP, hImgP);

          // Set height/zoom of view
          double Z = 25;
          H.at<double>(2,2) = Z;

          birdImage = frame.clone();

          //warp the image wrt the homography matrix
          warpPerspective(frame, birdImage, H, frame.size(),CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS);

          // Print birds eye view image
          namedWindow("Bird", CV_WINDOW_KEEPRATIO);
          resizeWindow("Bird", 500,500);
          imshow("Bird",birdImage);
           **/






          objPoints.push_back(objp);
          imgPoints.push_back(corner_pts);

        }
        else {
            qDebug("Failed image %d", i+1);
        }
        // Print image with detectec corners
        namedWindow("Image", CV_WINDOW_KEEPRATIO);
        resizeWindow("Image", 500,500);
        imshow("Image",frame);


        //waitKey(0);   //wait for 0 to be pressed

      }

    destroyAllWindows();




    /*** Camera calibration ***/

    Mat cameraMatrix,distCoeffs,R,T;

    double rmsCalib = calibrateCamera(objPoints, imgPoints, Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T);


    //Fish-eye calibration
    /*
    int flags = fisheye::CALIB_FIX_SKEW|fisheye::CALIB_RECOMPUTE_EXTRINSIC|fisheye::CALIB_FIX_K1|fisheye::CALIB_FIX_K2|fisheye::CALIB_FIX_K3|fisheye::CALIB_FIX_K4;
    //TermCriteria criteria(TermCriteria::EPS|TermCriteria::MAX_ITER, 30, 1e-6);
    double rmsFish = fisheye::calibrate(objPoints, imgPoints, Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T, flags);//, criteria);
    */




    //Print camera parameters
    cout << "cameraMatrix : " << cameraMatrix << endl;
    cout << "distCoeffs : " << distCoeffs << endl;
    cout << "Rotation vector : " << R << endl;
    cout << "Translation vector : " << T << endl;
    cout << "RMS CameraCalibration : " << rmsCalib << endl;
     //cout << "RMS Fisheye calibration : " << rmsFish << endl;




    /*** Undistorted image ***/
    Mat distortedIMG, undistortedIMG, gray2;
    distortedIMG = imread(images[7]);   //fetch image to undistort
    undistort(distortedIMG, undistortedIMG, cameraMatrix, distCoeffs);
    //fisheye::undistortImage(distortedIMG, undistortedIMG, cameraMatrix, distCoeffs);

    namedWindow("Distorted", CV_WINDOW_KEEPRATIO);  //create window
    resizeWindow("Distorted", 500,500);             //set window size
    imshow("Distorted",distortedIMG);

    namedWindow("Undistorted", CV_WINDOW_KEEPRATIO);
    resizeWindow("Undistorted", 500,500);
    imshow("Undistorted",undistortedIMG);




    /** Find homography **/
    //Creating gray scal cpy
    cvtColor(undistortedIMG,gray2,COLOR_BGR2GRAY);
    namedWindow("Gray", CV_WINDOW_KEEPRATIO);
    resizeWindow("Gray", 500,500);
    imshow("Gray",gray2);


    // Finding checker board corners
    success = findChessboardCorners(gray2, Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_FAST_CHECK + CALIB_CB_NORMALIZE_IMAGE);



    if(success)
    {
        qDebug("Success, Chessboard for bird's-eye view found");


        // refining pixel coordinates for given 2d points.
        cornerSubPix(gray2,corner_pts,Size(11,11), Size(-1,-1),TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 30, 0.0001 ));



        vector<Point2f> hImgP(4), hObjP(4);

        //Choose four corners on the chessboard
        hImgP[0] = corner_pts[0];
        hImgP[1] = corner_pts[board_w-1];
        hImgP[2] = corner_pts[(board_h-1)*board_w];
        hImgP[3] = corner_pts[(board_h-1)*board_w + board_w-1];



        hObjP[0].x = 0;
        hObjP[0].y = 0;

        hObjP[1].x = board_w -1;
        hObjP[1].y = 0;

        hObjP[2].x = 0;
        hObjP[2].y = board_h -1;

        hObjP[3].x = board_w -1;
        hObjP[3].y = board_h -1;

        // Calculate homography matrix
        Mat H = getPerspectiveTransform(hObjP, hImgP);

        // Set height/zoom of view
        double Z = 20;
        H.at<double>(2,2) = Z;

        birdImage = undistortedIMG.clone();

        //warp the image wrt the homography matrix
        warpPerspective(undistortedIMG, birdImage, H, frame.size(),CV_INTER_LINEAR | CV_WARP_INVERSE_MAP | CV_WARP_FILL_OUTLIERS);

        // Print birds eye view image
        namedWindow("Bird", CV_WINDOW_KEEPRATIO);
        resizeWindow("Bird", 500,500);
        imshow("Bird",birdImage);

    }









    //Stop clock
    auto t2 = high_resolution_clock::now();
    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    cout << ms_int.count() << "ms\n" << endl;




}

MainWindow::~MainWindow()
{
    delete ui;
}



