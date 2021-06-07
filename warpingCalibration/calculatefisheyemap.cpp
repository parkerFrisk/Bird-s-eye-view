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
void calculateFisheyeMap(Mat& cameraMatrix, Mat& distCoeffs, Size& imageSize, Mat& map1, Mat& map2)
{

    fisheye::initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), cameraMatrix , imageSize, CV_16SC2, map1, map2);

}
