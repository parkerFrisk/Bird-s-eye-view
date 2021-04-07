#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

#include <iostream>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_open_cam1_clicked();
    void on_pushButton_open_cam2_clicked();
    void on_pushButton_open_cam3_clicked();
    void on_pushButton_open_cam4_clicked();

    void on_pushButton_take_snapshot_clicked();

    void on_pushButton_close_cam_clicked();

    void update_window();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    VideoCapture cap;

    Mat frame;
    QImage qt_image;

    int writeImage = 0;
    bool test = true;
};

#endif // MAINWINDOW_H
