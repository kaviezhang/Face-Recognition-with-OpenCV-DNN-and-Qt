#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <QObject>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QDir>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QResource>
#include <opencv2/opencv.hpp>

class FaceDetector : public QObject
{
    Q_OBJECT
    QString facecascade_filename_;
    QString eyecascade_filename_;

    cv::Mat frame_;
    bool processAll_;
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;

    void process(cv::Mat frame);
    void scapped(cv::Mat frame);
    void loadFiles(cv::String faceCascadeFilename, cv::String eyesCascadeFilename);
    void queue(const cv::Mat & frame);
    void timerEvent(QTimerEvent* ev);
    static void matDeleter(void* mat);
    int series_num=0;
    int label=0;
    int label_array[7]={0,0,0,0,0,0,0};
    int no_identy=0;

public:
    FaceDetector(QObject *parent=0) : QObject(parent), processAll_(true)
    {
        facecascade_filename_ = "weights/haarcascade_frontalface_default.xml";
        eyecascade_filename_ = "weights/haarcascade_eye.xml";
        loadFiles(facecascade_filename_.toStdString().c_str(),
                  eyecascade_filename_.toStdString().c_str());
    }
    void setProcessMode(bool all);
    ~FaceDetector();
    void face_model_train();
    int face_model_predict(cv::Mat);
    QBasicTimer timer_;
    bool trained_label;
signals:
    void image_signal(const QImage&);
    void result_signal(int);
    void lock_signal();

public slots:
    void processFrame(const cv::Mat& frame);

};

#endif // FACEDETECTOR_H
