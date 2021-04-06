#ifndef CALCULATEFISHEYEMAP_H
#define CALCULATEFISHEYEMAP_H

#include <QMainWindow>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

using namespace cv;

void calculateFisheyeMap(Mat& cameraMatrix, Mat& distCoeffs, Size& imageSize, Mat& map1, Mat& map2);


#endif // CALCULATEFISHEYEMAP_H
