//#include <QDebug>
#include <QPainter>
#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>


std::string get_tegra_pipeline(int width, int height, int fps) {
    return "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(width) + ", height=(int)" +
           std::to_string(height) + ", format=(string)I420, framerate=(fraction)" + std::to_string(fps) +
           "/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Camera setup
    cv::VideoCapture cap(get_tegra_pipeline(1280, 720, 30), cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cout << "Connection failed";
    }

    // resize
    cv::Mat src;
    cap >> src;
    QSize windowSize(src.cols, src.rows);
    resize(windowSize);

    // Elapsed Time
    QTime time;
    time.start();

    // Print Cuda Environment
    int cudaDevices = cv::cuda::getCudaEnabledDeviceCount();
    if (cudaDevices > 0)
    {
        using namespace std;

        cv::cuda::DeviceInfo info(0);
        cout <<
            "[CUDA Device 0]" << endl <<
            "name: " << info.name() << endl <<
            "majorVersion: " << info.majorVersion() << endl <<
            "minorVersion: " << info.minorVersion() << endl <<
            "multiProcessorCount: " << info.multiProcessorCount() << endl <<
            "sharedMemPerBlock: " << info.sharedMemPerBlock() << endl <<
            "freeMemory: " << info.freeMemory() << endl <<
            "totalMemory: " << info.totalMemory() << endl <<
            "isCompatible: " << info.isCompatible() << endl <<
            "/*--------------*/" << endl;
    }

    // Normar CV
    cv::Mat frame, fliped, hsv, blur, edges;

//    // CUDA CV
//    cv::Mat frame;
//    cv::cuda::GpuMat fliped, hsv, blur, edges;

    // View video
    while (1) {
        // Get a new frame from camera
        cap >> frame;

        // Normar CV
        cv::flip(frame, fliped, -1);
        cv::cvtColor(fliped, hsv, CV_BGR2GRAY);
        cv::GaussianBlur(hsv, blur, cv::Size(7,7), 1.5);
        cv::Canny(blur, edges, 0.0, 40.0);
        cv::Mat dst = edges;

//        // CUDA CV
//        cv::cuda::flip(frame, fliped, -1);
//        cv::cuda::cvtColor(fliped, hsv, CV_BGR2GRAY);
//        cv::Ptr<cv::cuda::Filter> filter_g = cv::cuda::createGaussianFilter(hsv.type(), blur.type(), cv::Size(7,7), 1.5);
//        filter_g->apply(hsv, blur);
//        cv::Ptr<cv::cuda::CannyEdgeDetector> canny = cv::cuda::createCannyEdgeDetector(0.0, 100.0);
//        canny->detect(blur, edges);
//        cv::Mat dst(edges);

        // Draw
        imshow("Display window", dst);
        cv::waitKey(1);

        // Debug
        double elapsed_time = time.elapsed();
        static long elapsed_frame = 0;
        static double elapsed_time_z1 = 0.0;
        double time_diff = elapsed_time - elapsed_time_z1;
        double fps_sec = int(1000.0 / (time_diff));

        qDebug() << ("Frame:" + QString::number(elapsed_frame) + ", " + "Diff:" + QString::number(time_diff) + ", " + "FPS:" + QString::number(fps_sec));

        elapsed_frame += 1;
        elapsed_time_z1 = elapsed_time;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
