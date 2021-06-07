#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

using namespace cv;

void fisheyeUndistortion(Mat& distortedIMG, Mat& undistortedIMG, Mat& map1, Mat& map2);
void calculateFisheyeMap(Mat& cameraMatrix, Mat& distCoeffs, Size& imageSize, Mat& map1, Mat& map2);
void perspectiveHomograpy(Mat& undistortedIMG, int CHECKERBOARD[], Mat& H);


void floorPoints(Mat undistortedIMG, Mat& H);

void calculatePadding(Mat& src, Mat& M, Size& paddingSize, int& horizon);
void translationXY(Mat& src, Mat& M);
void humanDetectionHOG(Mat& img);


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
