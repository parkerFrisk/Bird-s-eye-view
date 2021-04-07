#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <string>
//#include <sstream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_open_cam1_clicked()
{
    // Create pipeline for cam1
    cap.open("udpsrc port=62464 ! application/x-rtp,payload=26,encoding-name=JPEG,media=video ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! appsink", CAP_ANY);

    // Check if stream is OK
    if(!cap.isOpened()) {
        cout << "cam1 NOT OK" << endl;
    }
    else {
        cout << "cam1 OK" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
}

void MainWindow::on_pushButton_open_cam2_clicked()
{
    // Create pipeline for cam2
    cap.open("udpsrc port=62465 ! application/x-rtp,payload=26,encoding-name=JPEG,media=video ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! appsink", CAP_ANY);

    // Check if stream is OK
    if(!cap.isOpened()) {
        cout << "cam2 NOT OK" << endl;
    }
    else {
        cout << "cam2 OK" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
}

void MainWindow::on_pushButton_open_cam3_clicked()
{
    // Create pipeline for cam3
    cap.open("udpsrc port=62466 ! application/x-rtp,payload=26,encoding-name=JPEG,media=video ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! appsink", CAP_ANY);

    // Check if stream is OK
    if(!cap.isOpened())
    {
        cout << "cam3 NOT OK" << endl;
    }
    else
    {
        cout << "cam3 OK" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
}

void MainWindow::on_pushButton_open_cam4_clicked()
{
    // Create pipeline for cam4
    cap.open("udpsrc port=62467 ! application/x-rtp,payload=26,encoding-name=JPEG,media=video ! rtpjpegdepay ! jpegparse ! jpegdec ! videoconvert ! appsink", CAP_ANY);

    // Check if stream is OK
    if(!cap.isOpened())
    {
        cout << "cam4 NOT OK" << endl;
    }
    else
    {
        cout << "cam4 OK" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
}

void MainWindow::on_pushButton_take_snapshot_clicked()
{
    // Take snapshot from current camera
    // OBS!!! Remember to change filename to the right camera no. and purpose of snapshot(i.e. flooralib, camCalib etc.)!
    writeImage ++;
    string filename = "/opt/images/floorCalib/floorCalibCam1" + to_string(writeImage) + ".png"; //cam2CalibImg_
    cvtColor(frame, frame, COLOR_BGR2RGB);
    imwrite(filename,frame);

}

void MainWindow::on_pushButton_close_cam_clicked()
{
    // Close current camera stream
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    cap.release();

    Mat image = Mat::zeros(frame.size(),CV_8UC3);
    qt_image = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);

    ui->cam->setPixmap(QPixmap::fromImage(qt_image));
    ui->cam->resize(ui->cam->pixmap()->size());

    cout << "camera is closed" << endl;

}

void MainWindow::update_window()
{
    // Show stream(frames) on screen
    cap >> frame;
    cvtColor(frame, frame, COLOR_BGR2RGB);
    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->cam->setPixmap(QPixmap::fromImage(qt_image));
    ui->cam->resize(ui->cam->pixmap()->size());

}
