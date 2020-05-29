#include "camera.h"
#include "cglobal.h"
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

Camera::~Camera()
{
}

void Camera::runSlot()
{
    camera_run=true;
    // TODO: clean up. Would be nice not to have nested `if` statements
    if (!videoCapture_ or !usingVideoCamera_)
    {
        if (usingVideoCamera_)
        {
            //videoCapture_.reset(new cv::VideoCapture(cameraIndex_));
            videoCapture_.reset(new cv::VideoCapture());
            //cv::VideoCapture video_;
            //bool sig=videoCapture_->open("rkisp device=%1 io-mode=1 ! video/x-raw,format=NV12,width=800,height=448,framerate=30/1 ! videoconvert ! appsink", cv::CAP_GSTREAMER);
            //String cam_rkisp="rkisp device=/dev/video1 io-mode=1 ! video/x-raw,format=NV12,width=500,height=300,framerate=30/1 ! videoconvert ! appsink";//MIPI
            //String cam_rkisp="rkisp device=/dev/video1 io-mode=1 ! video/x-raw,format=NV12,width=500,height=400,framerate=20/1 ! videoconvert ! appsink";//USB
            //bool sig=videoCapture_->open(cam_rkisp, cv::CAP_GSTREAMER);
            videoCapture_->open(0);
            //videoCapture_.reset(video_);
        }
        else
            videoCapture_.reset(new cv::VideoCapture(videoFileName_));
    }
    if (videoCapture_->isOpened())
    {
        timer_.start(100, this);
        emit started();
    }
}

void Camera::stopped()
{
    timer_.stop();
}

void Camera::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() != timer_.timerId())
        return;
    cv::Mat frame;
    if (!videoCapture_->read(frame)) // Blocks until a new frame is ready
    {
        timer_.stop();
        return;
    }
    //frame.convertTo(frame, frame.type(),1.0,50);
    //float scale=1.0/4;
    //Mat frame = Mat(dsize,CV_32S);
    resize(frame, frame, cv::Size(600,360));

    if(camera_run){emit matReady(frame);null_count=0;}
    else{
        null_count++;
        if(null_count<=50)
        {
            frame=Mat::zeros(Size(600,360),CV_8UC3);
            cv::putText(frame, "Unknown visitors", Point(200,100), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 255), 1, 4, 0);
            emit matReady(frame);
        }
    }
}

void Camera::usingVideoCameraSlot(bool value)
{
    usingVideoCamera_ = value;
}

void Camera::cameraIndexSlot(int index)
{
    cameraIndex_ = index;
}

void Camera::videoFileNameSlot(QString fileName)
{
    videoFileName_ = fileName.toStdString().c_str();
}
