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
 */
void translationXY(Mat& src, Mat& transf)
{

    int width = src.cols;
    int height = src.rows;

    // It is enough to find the corners of the original image to find
    // the padding bounds; The points are in clockwise order from origin
    int src_h = src.rows;
    int src_w = src.cols;
    Vec<Point2f, 4> init_pts, transf_pts;

    //select the boundry of the original image(These poins will be the extreme points after translation)
    init_pts[0] = Point2f (0, 0);
    init_pts[1] = Point2f (src_w, 0);
    init_pts[2] = Point2f (src_w, src_h);
    init_pts[3] = Point2f (0, src_h);

    // Invert translation matrix
    invert(transf, transf);

    //Compute transformation for only the boundry points
    perspectiveTransform(init_pts, transf_pts, transf);

//    cout << "transf_pts: " << transf_pts[0] << endl;
//    cout << "transf_pts: " << transf_pts[1] << endl;
//    cout << "transf_pts: " << transf_pts[2] << endl;
//    cout << "transf_pts: " << transf_pts[3] << endl;


    // find min and max points(Finds the highest and lowest boundry points for both x and y)
    int max_y;
    max_y = max(transf_pts[2].y, transf_pts[3].y);

    //qDebug(" max Y: %d",max_y);

    // add translation to transformation matrix to shift the image to bottom center
    int anchor_x = 0;
    int anchor_y = 0;
    Mat transl_transf = Mat::eye(3, 3, CV_32F);

    //Translation X
    int x =abs(transf_pts[2].x-transf_pts[3].x);
    anchor_x = ((width - x)/2) + x - transf_pts[2].x;
    transl_transf.at<float>(0,2) += anchor_x;

    //Translation Y
    anchor_y = height - max_y;
    transl_transf.at<float>(1,2) += anchor_y;

    Mat Temp;
    transf.convertTo(Temp, CV_32F);
    transf = transl_transf * Temp;
    transf /= transf.at<float>(2, 2);

    // Invert translation matrix back to normal
    invert(transf, transf);

}
