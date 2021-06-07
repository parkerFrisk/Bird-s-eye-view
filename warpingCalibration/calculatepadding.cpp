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

/*
 * Calculate the padding size required for including the entire image.
 * Is to be done before Warpperspective
 *
 *  //Horizion crop
 * int horizon = 100; //The horizon of the image calculated from the top of the image
    Rect crop_region(0, 0, undistortedIMG.size().width, horizon);
    undistortedIMG(crop_region).setTo(cv::Scalar(0,0,0));


    namedWindow("horizon crop", CV_WINDOW_KEEPRATIO);
    resizeWindow("horizon crop", 1000,1000);
    imshow("horizon crop", undistortedIMG);
    waitKey();
 */
void calculatePadding(Mat& src, Mat& transf, Size& paddingSize, int& horizon)
{

    int width = 700;
    int height = 500;
    int borderMode = BORDER_CONSTANT;
    const Scalar borderValue = Scalar();


    // It is enough to find the corners of the original image to find
    // the padding bounds; The points are in clockwise order from origin
    int src_h = src.rows;
    int src_w = src.cols;
    Vec<Point2f, 4> init_pts, transf_pts;

    //select the boundry of the original image(These poins will be the extreme points after translation)
    init_pts[0] = Point2f (0, horizon);
    init_pts[1] = Point2f (src_w, horizon);
    init_pts[2] = Point2f (src_w, src_h);
    init_pts[3] = Point2f (0, src_h);

    // Invert translation matrix
    invert(transf, transf);

    //Compute transformation for only the boundry points
    perspectiveTransform(init_pts, transf_pts, transf);

    cout << "transf_pts: " << transf_pts[0] << endl;
    cout << "transf_pts: " << transf_pts[1] << endl;
    cout << "transf_pts: " << transf_pts[2] << endl;
    cout << "transf_pts: " << transf_pts[3] << endl;


    // find min and max points(Finds the highest and lowest boundry points for both x and y)
    int min_x, min_y, max_x, max_y;
    /*min_x = floor(min(
        min(transf_pts[0].x, transf_pts[1].x),
        min(transf_pts[2].x, transf_pts[3].x)));

    max_x = ceil(max(
        max(transf_pts[0].x, transf_pts[1].x),
        max(transf_pts[2].x, transf_pts[3].x)));
    */
    /*
    if(transf_pts[1].y > transf_pts[0].y) { //If we have a trapezoid with high left corner
        qDebug("High left");
        min_x = transf_pts[3].x + abs(transf_pts[1].x - transf_pts[2].x)*(-1);
        max_x = transf_pts[1].x;
    }
    else if (transf_pts[0].y > transf_pts[1].y) { //If we have a trapezoid with high right corner
        qDebug("High left");
        min_x = transf_pts[0].x;
        max_x = transf_pts[2].x + abs(transf_pts[3].x - transf_pts[0].x);
    }
    else { //If we have a trapezoid with equal high corners
        min_x = transf_pts[0].x;
        max_x = transf_pts[1].x;
    }
    */
    min_x = transf_pts[3].x-((width-abs(transf_pts[2].x-transf_pts[3].x))/2)-1;
    max_x = transf_pts[2].x+((width-abs(transf_pts[2].x-transf_pts[3].x))/2);



    /*min_y = floor(min(
        min(transf_pts[0].y, transf_pts[1].y),
        min(transf_pts[2].y, transf_pts[3].y)));
    */
    min_y = floor(//min(
            max(transf_pts[0].y, transf_pts[1].y));//,
            //min(transf_pts[2].y, transf_pts[3].y)));
    max_y = ceil(max(
        max(transf_pts[0].y, transf_pts[1].y),
        max(transf_pts[2].y, transf_pts[3].y)));

    //qDebug("min X: %d, max X: %d, min Y: %d, max Y: %d",min_x,max_x,min_y,max_y);

    // add translation to transformation matrix to shift negative values in the image to positive values
    int anchor_x = 0;
    int anchor_y = 0;
    Mat transl_transf = Mat::eye(3, 3, CV_32F);
    if (min_x < 0)
    {
        anchor_x = -min_x;
        transl_transf.at<float>(0,2) += anchor_x;
    }
    if (min_y < 0)
    {
        //anchor_y = -min_y;
        //transl_transf.at<float>(1,2) += anchor_y;
        if(height-max_y){
            anchor_y = height- max_y;
            transl_transf.at<float>(1,2) += anchor_y;
        }
    }



    Mat Temp;
    transf.convertTo(Temp, CV_32F);
    transf = transl_transf * Temp;
    transf /= transf.at<float>(2, 2);



    // create padded destination image
    int pad_top = anchor_y;
    int pad_bot = max(max_y, src_h)- src_h;
    int pad_left = anchor_x;
    int pad_right = max(max_x, src_w)- src_w;
    Mat src_padded;
    copyMakeBorder(src, src_padded, pad_top, pad_bot, pad_left, pad_right, borderMode, borderValue);

    //If the image is smaller than the original image
    if(max_y < src_h){
        Rect crop_region(0, 0, src_padded.size().width, max_y + anchor_y);
        src_padded = src_padded(crop_region);
    }
    if(max_x < src_w){
        Rect crop_region(0, 0, max_x + anchor_x, src_padded.size().height);
        src_padded = src_padded(crop_region);
    }

    // transform src into larger window
    int src_pad_h = src_padded.rows;
    int src_pad_w = src_padded.cols;
    paddingSize = Size (src_pad_w, src_pad_h);



    namedWindow("padded", CV_WINDOW_KEEPRATIO);
    resizeWindow("padded", 1000,1000);
    imshow("padded", src_padded);
    waitKey();


}
