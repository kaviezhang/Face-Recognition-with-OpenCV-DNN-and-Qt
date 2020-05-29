#include "faceDetector.h"
#include <opencv2/face.hpp>
#include <opencv2/face/facerec.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "cglobal.h"


using namespace cv;
using namespace cv::face;
using namespace std;

void FaceDetector::loadFiles(cv::String faceCascadeFilename,
                       cv::String eyeCascadeFilename)
{
    // TODO: Add in a try catch statement here
    if( !faceCascade.load( faceCascadeFilename ) )
    {
        std::cout << "Error Loading" << faceCascadeFilename << std::endl;
    }

    if( !eyeCascade.load( eyeCascadeFilename ) )
    {
        std::cout << "Error Loading" << eyeCascadeFilename << std::endl;
    }
    //load the face recognizer model
    model = face::LBPHFaceRecognizer::create();

    //FACE MODEL
    string model_txt = "weights/resnet50_256.prototxt";
    string model_bin = "weights/resnet50_256.caffemodel";
    try {
        vggface = dnn::readNetFromCaffe(model_txt, model_bin);
    }
    catch (cv::Exception &ee) {
        cout << ee.what();
        //QMessageBox::warning(this, "Exception", ee.what());
        if (vggface.empty()) {
            cout << "Can't load the network by using the flowing files.";
            //QMessageBox::warning(this, "Exception", "Can't load the network by using the flowing files.");
        }
    }

    face_model_train();
}

FaceDetector::~FaceDetector()
{

}

void FaceDetector::processFrame(const cv::Mat &frame)
{
    Ptr<face::BasicFaceRecognizer> model;
    if (processAll_)
        process(frame);
    else
        scapped(frame);
}

void FaceDetector::setProcessMode(bool mode)
{
    processAll_ = mode;
}

void FaceDetector::process(cv::Mat frame)
{
    cv::Mat grey_image;
    cv::Mat raw_frame=frame.clone();
    cv::cvtColor(frame, grey_image, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grey_image, grey_image);

    std::vector<cv::Rect> faces;
    // Calculate the camera size and set the size to 1/8 of screen height
    faceCascade.detectMultiScale(grey_image, faces, 1.1, 2,  0|cv::CASCADE_SCALE_IMAGE,
                                 cv::Size(frame.cols/4, frame.rows/4)); // Minimum size of obj
    //-- Draw rectangles around faces
    int predict_label=0;

    //-- Draw rectangles around faces
    if (faces.size()==1)
    {
        if(series_num==0)
        for( size_t i = 0; i < faces.size(); i++)
        {
            cv::rectangle(frame, faces[i], cv::Scalar( 255, 0, 255 ));
            cv::Mat faceROI = frame( faces[i] );
            std::vector<cv::Rect> eyes;

            //-- In each face, detect eyes
            if(this->trained_label==true)
            {
            eyeCascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

            if(eyes.size()>0)
            {
                float dis_x=eyes[0].x+eyes[0].width*0.5-eyes[1].x-eyes[1].width*0.5;
                float dis_y=eyes[0].y+eyes[0].height*0.5-eyes[1].y-eyes[1].height*0.5;
                float dis=sqrt(pow(dis_x,2)+pow(dis_y,2));
                float radius0=(eyes[0].width + eyes[0].height) *0.25;
                float radius1=(eyes[1].width + eyes[1].height) *0.25;
                if(1)
                {
                    //define the face recognizer model
                    cv::Mat faceROI = frame( faces[i] );
                    cv::Mat rawfaceROI = raw_frame( faces[i] );
                    //cv::cvtColor(rawfaceROI, rawfaceROI, cv::COLOR_BGR2GRAY);
                    resize(rawfaceROI,rawfaceROI,cv::Size(224,224));
                    predict_label=face_model_predict(rawfaceROI);

                    label_array[predict_label]++;
                    series_num++;
                    label=predict_label;
                    camera_run=false;
                }
            }
            }//train_label
        }
    }
    else
    {
        if(series_num>0)
        {
            int maxposition=max_element(label_array+1,label_array+7)-label_array;
            if(label_array[maxposition]>=label_array[0])
            {emit result_signal(maxposition);series_num=0;no_identy=0;}
            else
            {
            if(no_identy>10)
            {   emit lock_signal();no_identy=0;series_num=0;}
            else
            {   camera_run=true;
                series_num=0;
                no_identy++;}
            }
            memset(label_array,0,sizeof(int)*7);
        }
    }
    putText(frame,QString::number(predict_label).toStdString(),Point(50,50),FONT_HERSHEY_SIMPLEX,1.5,Scalar(0,0,255),2);

    Rect rect(192,0,216,360);
    frame=frame(rect);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    cv::transpose(frame, frame);
    cv::flip(frame,frame,1);
    resize(frame, frame, cv::Size(600,360));

    const QImage image((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step,
                       QImage::Format_RGB888, &matDeleter, new cv::Mat(frame));
    image.rgbSwapped();
    Q_ASSERT(image.constBits() == frame.data);
    emit image_signal(image);
}
void FaceDetector::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() != timer_.timerId())
        return;
    process(frame_);
    frame_.release();
    timer_.stop();
}

void FaceDetector::queue(const cv::Mat &frame)
{
    if (!frame.empty())
        qDebug() << "Converter dropped frame !";

    frame_ = frame;
    if (!timer_.isActive())
        timer_.start(0, this);
}

void FaceDetector::matDeleter(void *mat)
{
    delete static_cast<cv::Mat*>(mat);
}

void FaceDetector::scapped(cv::Mat frame)
{
    cv::Mat grey_image;
    cv::Mat raw_frame=frame.clone();
    cv::cvtColor(frame, grey_image, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grey_image, grey_image);

    std::vector<cv::Rect> faces;
    // Calculate the camera size and set the size to 1/8 of screen height
    faceCascade.detectMultiScale(grey_image, faces, 1.1, 2,  0|cv::CASCADE_SCALE_IMAGE,
                                 cv::Size(frame.cols/4, frame.rows/4)); // Minimum size of obj
    //-- Draw rectangles around faces
    if (faces.size()==1)
    {
        for( size_t i = 0; i < faces.size(); i++)
        {
            cv::rectangle(frame, faces[i], cv::Scalar( 255, 0, 255 ));
            cv::Mat faceROI = frame( faces[i] );
            std::vector<cv::Rect> eyes;

            //-- In each face, detect eyes
            eyeCascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

            if(eyes.size()==2)
            {
                float dis_x=eyes[0].x+eyes[0].width*0.5-eyes[1].x-eyes[1].width*0.5;
                float dis_y=eyes[0].y+eyes[0].height*0.5-eyes[1].y-eyes[1].height*0.5;
                float dis=sqrt(pow(dis_x,2)+pow(dis_y,2));
                float radius0=(eyes[0].width + eyes[0].height) *0.25;
                float radius1=(eyes[1].width + eyes[1].height) *0.25;
                if((radius0+radius1<=dis) && (dis_x<=5))
                {
                    for( size_t j = 0; j < eyes.size(); j++)
                    {
                        cv::Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5,
                                  faces[i].y + eyes[j].y + eyes[j].height*0.5 );
                        int radius = cvRound( (eyes[j].width + eyes[j].height) *0.25);
                        circle( frame, center, radius, cv::Scalar( 255, 0, 0 ), 4, 8, 0);
                    }
                    //imwrite("ffffff.jpg", faceROI);

                    if(user_num!=0){
                    QString dir_str = "trainset/user"+QString::number(user_num);
                    QDir dir;
                    if(!dir.exists(dir_str))
                    {
                        dir.mkpath(dir_str);
                    }

                    QString img_path=dir_str+"/0"+QString::number(usergraph_count)+".jpg";
                    cv::Mat rawfaceROI = raw_frame( faces[i] );
                    cv::resize(rawfaceROI,rawfaceROI,cv::Size(224,224));

                    imwrite(img_path.toStdString(),rawfaceROI);
                    usergraph_count++;

                    if(usergraph_count==1){
                    Mat temp;
                    cv::transpose(rawfaceROI, temp);
                    cv::flip(temp,temp,1);
                    img_path=dir_str+"/0"+"xxx.jpg";
                    imwrite(img_path.toStdString(),temp);}
                    }
                }
            }
        }
    }

    Rect rect(192,0,216,360);
    frame=frame(rect);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    cv::transpose(frame, frame);
    cv::flip(frame,frame,1);
    resize(frame, frame, cv::Size(600,360));

    const QImage image((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step,
                       QImage::Format_RGB888, &matDeleter, new cv::Mat(frame));
    image.rgbSwapped();
    Q_ASSERT(image.constBits() == frame.data);
    emit image_signal(image);
}

void FaceDetector::face_model_train()
{
    vector<Mat> images;
    vector<int> labels;

    for(int item=1;item<=6;item++)
    {
        QString dir_str = "trainset/user"+QString::number(item);
        QDir dir;
        if(dir.exists(dir_str))
        {
            for(int jtem=1;jtem<=1;jtem++)
            {
                QString abo_dir=dir_str+"/0"+QString::number(jtem)+".jpg";
                QFile file(abo_dir);
                if(file.exists())
                {
                    images.push_back(imread(abo_dir.toStdString(),IMREAD_GRAYSCALE));
                    labels.push_back(item);

                    Mat tset = imread(abo_dir.toStdString());
                    if(tset.channels() == 4)cvtColor(tset, tset, COLOR_BGRA2BGR);
                    Mat testBlob = dnn::blobFromImage(tset, 1.0f, Size(224, 224), Scalar(91, 104, 131), false, false);
                    vggface.setInput(testBlob, "data");						//set the network input
                    Mat feature_map = vggface.forward("feat_extract");
                    abo_dir=dir_str+"/0"+QString::number(jtem)+".mat";

                    FileStorage fs(abo_dir.toStdString(),FileStorage::WRITE);
                    fs<<"feature"<<feature_map;
                    fs.release();
                }
            }
        }
    }
    if(images.size()!=0)
    {
        model->train(images, labels);
        this->trained_label=true;
    }
    else{this->trained_label=false;}
}

int FaceDetector::face_model_predict(cv::Mat tset)
{
    if(tset.channels() == 4)cvtColor(tset, tset, COLOR_BGRA2BGR);
    Mat testBlob = dnn::blobFromImage(tset, 1.0f, Size(224, 224), Scalar(91, 104, 131), false, false);
    vggface.setInput(testBlob, "data");//set the network input
    Mat feature_map = vggface.forward("feat_extract");
    double odis=150;
    int label=0;

    for(int item=1;item<=6;item++)
    {
        QString dir_str = "trainset/user"+QString::number(item);
        QDir dir;
        if(dir.exists(dir_str))
        {
            for(int jtem=0;jtem<=0;jtem++)
            {
                QString abo_dir=dir_str+"/0"+QString::number(jtem+1)+".mat";
                QFile file(abo_dir);
                if(file.exists())
                {
                    FileStorage fs(abo_dir.toStdString(),FileStorage::READ);
                    Mat feature_com;
                    fs["feature"]>>feature_com;
                    fs.release();

                    double odis_n=cv::norm(feature_map,feature_com,NORM_L2);
                    printf("%f",odis);
                    if(odis_n<odis){label=item;odis=odis_n;}
                }
            }
        }
    }
    printf("label=%d,oids=%f\n",label,odis);
    return label;
}
