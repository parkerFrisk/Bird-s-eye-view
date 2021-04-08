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
//const char* keys =
//        "{ help h |                          | Print help message. }"
//        "{ input1 | /opt/images/img1.png | Path to input image 1. }"
//        "{ input2 | /opt/images/img2.png | Path to input image 2. }";


/** @function main */
int main( int argc, char** argv )
{



    // Load the images
    Mat image1 = imread("/opt/images/topViewCorners/topViewCam2Corner2.png");
    Mat image2 = imread("/opt/images/topViewCorners/topViewCam3Corner2.png");

    if( image1.empty() ) { cout << "Error loading src1" << endl; return -1; }
    if( image2.empty() ) { cout << "Error loading src2" << endl; return -1; }

    // Convert to Grayscale
    cvtColor(image1, image1, CV_RGB2GRAY);
    cvtColor(image2, image2, CV_RGB2GRAY);

    // Add padding
    Scalar value(0, 0, 0);
    int borderType = BORDER_CONSTANT;

    copyMakeBorder(image1, image1, 0, 1000, 500, 500, borderType, value);
    copyMakeBorder(image2, image2, 0, 1000, 0, 1000, borderType, value);

    imwrite("/opt/images/topViewCorners/topViewCam2Corner2Padded.png", image1);
    imwrite("/opt/images/topViewCorners/topViewCam3Corner2Padded.png", image2);




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




    Mat src = image2;
    Mat dst = image1;

    vector<Point_<float>> triSrc, triDst;
    triSrc.push_back(cornersImg2[0]);
    triSrc.push_back(cornersImg2[3]);
    triSrc.push_back(cornersImg2[8]);

    triDst.push_back(cornersImg1[0]);
    triDst.push_back(cornersImg1[3]);
    triDst.push_back(cornersImg1[8]);

    Mat warp_mat = getAffineTransform( triSrc, triDst );
    Mat warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    warpAffine( src, warp_dst, warp_mat, warp_dst.size()*3 );
    cout << "affine warp OK" << endl;

//    Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
//    double angle = -50.0;
//    double scale = 0;

//    Mat rot_mat = getRotationMatrix2D( center, angle, scale );
//    Mat warp_rotate_dst;

//    warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );
//    imshow( "Source image", src );
//    imshow( "Warp", warp_dst );
    imwrite("/opt/images/topViewCorners/warp.png", warp_dst);
//    imshow( "Warp + Rotate", warp_rotate_dst );
//    imwrite("/opt/images/topViewCorners/warpRot.png", warp_rotate_dst);




    float alpha = 0.5;
    float beta = 1.0 - alpha;
    float gamma = 0.0;

    Mat image12;
    addWeighted(image1,alpha,warp_dst,beta,gamma,image12);





//    //-- Step 1: Detect the keypoints using SURF Detector
//    int minHessian = 400;
//    //SurfFeatureDetector detector( minHessian );
//    Ptr<SURF> detector = SURF::create(minHessian);

//    std::vector< KeyPoint > keypoints_object, keypoints_scene;

//    detector->detect(gray_image1, keypoints_object);
//    detector->detect(gray_image2, keypoints_scene);

//    //-- Step 2: Calculate descriptors (feature vectors)
//    //SurfDescriptorExtractor extractor;
//    Ptr<SURF> extractor = SURF::create();

//    Mat descriptors_object, descriptors_scene;

//    extractor->compute( gray_image1, keypoints_object, descriptors_object );
//    extractor->compute( gray_image2, keypoints_scene, descriptors_scene );

//    //-- Step 3: Matching descriptor vectors using FLANN matcher
//    FlannBasedMatcher matcher;
//    std::vector< DMatch > matches;
//    matcher.match( descriptors_object, descriptors_scene, matches );

////    double max_dist = 0; double min_dist = 100;

////    //-- Quick calculation of max and min distances between keypoints
////    for( int i = 0; i < descriptors_object.rows; i++ )
////    { double dist = matches[i].distance;
////        if( dist < min_dist ) min_dist = dist;
////        if( dist > max_dist ) max_dist = dist;
////    }

//    //printf("-- Max dist : %f \n", max_dist );
//    //printf("-- Min dist : %f \n", min_dist );

//    //-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
//    std::vector< DMatch > good_matches;

//    for( int i = 0; i < descriptors_object.rows; i++ )
//    {
//        if( matches[i].distance < 3*min_dist ) {
//            good_matches.push_back( matches[i]);
//        }
//    }
//    std::vector< Point2f > obj;
//    std::vector< Point2f > scene;

//    for( int i = 0; i < good_matches.size(); i++ )
//        {
//            //-- Get the keypoints from the good matches
//            obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
//            scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
//        }

//    // Find the Homography Matrix
//    Mat H = findHomography( obj, scene, RANSAC );
////    cout << "H: " << H << endl;

//    Mat warped;
//    warpPerspective(image1,warped,H,cv::Size(image1.cols+image2.cols,image1.rows+image2.rows));
//    imwrite("/opt/images/topView/warped.png",warped);
//    qDebug("warp OK");

////    Mat image2_bgra;
////    cvtColor(image2, image2_bgra, CV_BGR2BGRA);

////    // find all white pixel and set alpha value to zero:
////    for (int y = 0; y < image2_bgra.rows; ++y)
////    for (int x = 0; x < image2_bgra.cols; ++x)
////    {
////        Vec4b & pixel = image2_bgra.at<cv::Vec4b>(y, x);
////        // if pixel is black
////        if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0)
////        {
////            // set alpha to zero:
////            pixel[3] = 0;
////        }
////    }
////    imwrite("/opt/images/topView/removedBlack.png",image2_bgra);
////    qDebug("removedBlack OK");

//    Mat combined;
//    cvtColor(warped, combined, CV_BGR2BGRA);
//    Mat half(combined,cv::Rect(0,0,image2.cols,image2.rows));
//    image2.copyTo(half);
//    qDebug("copy OK");

//    imwrite("/opt/images/topView/combined.png",combined);
//    qDebug("combined OK");
////    waitKey(0);

////    double alpha = 0.5;
////    double beta = 1 - alpha;
////    double gamma = 0.0;

////    Mat blended;
////    addWeighted(warped, alpha, image2, beta, gamma, blended);
////    imwrite("/opt/images/topView/blended.png",blended);
////    qDebug("blended OK");

    return 0;
}



#else
int main()
{
    std::cout << "This tutorial code needs the xfeatures2d contrib module to be run." << std::endl;
    return 0;
}
#endif


