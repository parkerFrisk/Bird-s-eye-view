#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    Size imageSize = Size(640, 480);
    // Defining the dimensions of checkerboard
    int board_w = 4; //8 /7 /6 /4
    int board_h = 3; //6 /5 /4 /3
    int CHECKERBOARD[2]{board_w,board_h};

    //Get calibration parameters
    FileStorage fileRead("/home/ccs/Documents/cam4Calibration2021-03-26.xml", FileStorage::READ);

    double rms;
    fileRead["rms"] >> rms;

    Mat cameraMatrix, distCoeffs;
    fileRead["cameraMatrix"] >> cameraMatrix;
    fileRead["distCoeffs"] >> distCoeffs;

    cout << "RMS: " << rms << endl
         << "Camera matrix: " << cameraMatrix << endl
         << "Distortion coeffs: " << distCoeffs << endl;




    /*** Fisheye Mapping (is only done once as a part of the calibration of each camera)**/
    Mat map1, map2;
    calculateFisheyeMap(cameraMatrix, distCoeffs, imageSize, map1, map2);


    /*** Undistorted image (Is done for one image with floored chessboard per camera)***/
    Mat distortedIMG, undistortedIMG;
    //distortedIMG = imread("/home/ccs/Pictures/floor/2021-04-09/floor4.png");   //fetch image with floored chessboard
    distortedIMG = imread("/home/ccs/Pictures/corner/2021-03-31/corner7.png");

//    //Should be used when calculating perspective homography
//    Mat gray;
//    cvtColor(distortedIMG,gray,COLOR_BGR2GRAY);
//    fisheyeUndistortion(gray, undistortedIMG, map1, map2);

    fisheyeUndistortion(distortedIMG, undistortedIMG, map1, map2);

      //Save undistorted image
//    string filename_undistort = "/home/ccs/Pictures/undistorted/corner2_undistorted.png";
//    imwrite(filename_undistort,undistortedIMG);
//    qDebug("undistorted saved");
//    waitKey();



    /*** Calculate perspective homography ***/
    Mat H;
//    perspectiveHomograpy(undistortedIMG, CHECKERBOARD, H);

////    //warpPerspective without translation in xy
//    Mat crop_warped;
//    warpPerspective(undistortedIMG, crop_warped, H, crop_warped.size(), WARP_INVERSE_MAP);
//    namedWindow("standard crop", CV_WINDOW_KEEPRATIO);
//    resizeWindow("standard crop", 500,500);
//    imshow("standard crop", crop_warped);
//    cout << "Widht: " << crop_warped.cols << endl;
//    cout << "Height: " << crop_warped.rows << endl;
//    cout << "H: " << H << endl;
//    waitKey();

//    translationXY(undistortedIMG, H);

    //Get perspective homography and paddingSize from file
    FileStorage fileH("/home/ccs/Pictures/topViewImages/params/2021-04-09/topView_H_Cam4.xml", FileStorage::READ);
    fileH["topViewH"] >> H;
    cout << "H: " << H << endl;



    /*** Changeperspective to Bird's-eye view ***/
    Mat src_warped;
    warpPerspective(undistortedIMG, src_warped, H, src_warped.size(), WARP_INVERSE_MAP);
    namedWindow("Bird's-eye view", CV_WINDOW_KEEPRATIO);
    resizeWindow("Bird's-eye view", 500,500);
    imshow("Bird's-eye view", src_warped);
    cout << "Widht: " << src_warped.cols << endl;
    cout << "Height: " << src_warped.rows << endl;
    waitKey();



    //Save topView image
    string filename_topImg = "/home/ccs/Pictures/topViewImages/2021-04-09/topViewImgCam4_R.png";
    imwrite(filename_topImg,src_warped);


    // Wrtie topview params to file
//    FileStorage file("/home/ccs/Pictures/topViewImages/params/2021-04-09/topView_H_Cam4.xml", cv::FileStorage::WRITE);
//    file << "topViewH" << H;
//    file.release();


    // Wrtie all calaibration params to file
//    FileStorage file("/home/ccs/Pictures/calibrationParameters/cam1calibrationParameters.xml", cv::FileStorage::WRITE);
//    file << "map1" << map1; //map1 and map2 are used for remaping fish-eye
//    file << "map2" << map2;
//    file << "topViewH" << H;
//    file.release();



    /*** Human detetction***/
//    Mat img = imread(images[2]);
//    humanDetectionHOG(img);
//    imshow("video capture", img);
//    waitKey();


}

MainWindow::~MainWindow()
{
    delete ui;
}
