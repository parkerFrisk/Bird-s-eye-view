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

#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;




int main( int argc, char** argv )
{

    vector<String> images;
    string path = "/opt/images/img*.png";
    glob(path, images);

    vector<Mat> imgs;
    Mat img1 = imread(images[0]);
    Mat img2 = imread(images[1]);
//    Mat img3 = imread(images[2]);
//    Mat img4 = imread(images[3]);
//    Mat img5 = imread(images[4]);
//    Mat img6 = imread(images[5]);
//    Mat img7 = imread(images[6]);
//    Mat img8 = imread(images[7]);

    imgs.push_back(img1);
    imgs.push_back(img2);
//    imgs.push_back(img3);
//    imgs.push_back(img4);
//    imgs.push_back(img5);
//    imgs.push_back(img6);
//    imgs.push_back(img7);
//    imgs.push_back(img8);

    Stitcher::Mode mode = Stitcher::SCANS;
    Mat pano;
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    Stitcher::Status status = stitcher->stitch(imgs, pano);
    cout << "status: " << status << endl;

    imwrite("/opt/images/stitchResult.png",pano);

    return 0;
}



#else
int main()
{
    std::cout << "This tutorial code needs the xfeatures2d contrib module to be run." << std::endl;
    return 0;
}
#endif



