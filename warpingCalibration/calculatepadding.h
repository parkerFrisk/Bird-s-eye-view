#ifndef CALCULATEPADDING_H
#define CALCULATEPADDING_H

#include <QMainWindow>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

using namespace cv;

/// Calculate the padding size required for including the entire image.
void calculatePadding(Mat& src, Mat& M, Size& paddingSize, int& horizon);


#endif // CALCULATEPADDING_H
