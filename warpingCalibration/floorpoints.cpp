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

void floorPoints(Mat undistortedIMG, Mat& H)
{

    //undistortedIMG = imread("/home/ccs/Pictures/floor/2021-04-12/red.png");

    Mat tempIMG = undistortedIMG;
    medianBlur(tempIMG,undistortedIMG,15);

    cvtColor(tempIMG,tempIMG,COLOR_RGB2HSV);

    //Specify the color range for the HSV written in BGR
    Scalar red_lower = Scalar(3,64,98);
    Scalar red_upper = Scalar(72,184,248);
    Mat frame_threshold;

    inRange(tempIMG, red_lower, red_upper, frame_threshold);

    namedWindow("threshold", CV_WINDOW_KEEPRATIO);
    resizeWindow("threshold", 500,500);
    imshow("threshold",frame_threshold);
    waitKey(0);

    Mat labels;
    Mat stats;
    Mat centroids;
    connectedComponentsWithStats(frame_threshold,labels, stats, centroids);

    //First centrioid is the centrioid of the image and is therefore removed
    centroids = centroids(Range(1,centroids.rows), Range::all()).clone();
    cout << "centroids: " << centroids << endl;


    for (int var = 0; var < centroids.rows; ++var) {
        circle( undistortedIMG, Point( centroids.at<double>(var,0), centroids.at<double>(var,1) ), 5, Scalar( 221, 0, 0 ), 0, 0 );
    }

     namedWindow("result", CV_WINDOW_KEEPRATIO);
     resizeWindow("result", 500,500);
     imshow("result",undistortedIMG);
     waitKey(0);


}
