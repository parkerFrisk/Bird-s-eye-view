#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <string>
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
    int board_w = 8;
    int board_h = 14;
    int CHECKERBOARD[2]{board_w,board_h};


    /*** Read calibration parameters ***/
    FileStorage fileRead("/home/ccs/Documents/cam1Calibration2021-03-23.xml", FileStorage::READ);

    double rms;
    fileRead["rms"] >> rms;

    Mat cameraMatrix, distCoeffs;
    fileRead["cameraMatrix"] >> cameraMatrix;
    fileRead["distCoeffs"] >> distCoeffs;

    cout << "RMS: " << rms << endl
         << "Camera matrix: " << cameraMatrix << endl
         << "Distortion coeffs: " << distCoeffs << endl;



    /*** Fisheye Mapping **/
    Mat map1, map2;
    calculateFisheyeMap(cameraMatrix, distCoeffs, imageSize, map1, map2);



    /*** Undistort image ***/
    Mat distortedIMG, undistortedIMG;
    distortedIMG = imread("/home/ccs/Pictures/floor/2021-04-13/cam1.png");   //fetch image with floored chessboard

    //Should be used when calculating perspective homography
    Mat gray;
    cvtColor(distortedIMG,gray,COLOR_BGR2GRAY);
    fisheyeUndistortion(gray, undistortedIMG, map1, map2);



    /*** Calculate perspective homography ***/
    Mat H;
    perspectiveHomograpy(undistortedIMG, CHECKERBOARD, H);
    translationXY(undistortedIMG, H);


    /*** Wrtie warping calaibration params to file ***/
    FileStorage file("/home/ccs/Pictures/warpingCalibrationParameters/cam1calibrationParameters.xml", cv::FileStorage::WRITE);
    file << "map1" << map1; //map1 and map2 are used for remaping fish-eye
    file << "map2" << map2;
    file << "topViewH" << H;
    file.release();




    /*** Fetch corner images ***/
    Mat distortedIMG_L, distortedIMG_R, undistortedIMG_L, undistortedIMG_R;
    distortedIMG_L = imread("/home/ccs/Pictures/corner/2021-04-23/corner8.png"); //fetch images with chessboard on the floor in the left corner
    distortedIMG_R = imread("/home/ccs/Pictures/corner/2021-04-23/corner1.png"); //fetch images with chessboard on the floor in the left corner
    fisheyeUndistortion(distortedIMG_L, undistortedIMG_L, map1, map2);
    fisheyeUndistortion(distortedIMG_R, undistortedIMG_R, map1, map2);



    /*** Changeperspective to Bird's-eye view ***/
    Mat src_warped_L, src_warped_R;
    warpPerspective(undistortedIMG_L, src_warped_L, H, src_warped_L.size(), WARP_INVERSE_MAP);
    warpPerspective(undistortedIMG_R, src_warped_R, H, src_warped_R.size(), WARP_INVERSE_MAP);

    namedWindow("Bird's-eye view Left", CV_WINDOW_KEEPRATIO);
    resizeWindow("Bird's-eye view Left", 500,500);
    imshow("Bird's-eye view Left", src_warped_L);

    namedWindow("Bird's-eye view Right", CV_WINDOW_KEEPRATIO);
    resizeWindow("Bird's-eye view Right", 500,500);
    imshow("Bird's-eye view Right", src_warped_R);
    waitKey();



    /*** Save topView images ***/
    string filename_topImg_L = "/home/ccs/Pictures/topViewImages/2021-04-16-2/topViewImgCam1_L.png";
    imwrite(filename_topImg_L,src_warped_L);

    string filename_topImg_R = "/home/ccs/Pictures/topViewImages/2021-04-16-2/topViewImgCam1_R.png";
    imwrite(filename_topImg_R,src_warped_R);







    waitKey();


}

MainWindow::~MainWindow()
{
    delete ui;
}
