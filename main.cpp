#include "widget.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda_types.hpp>
#include <opencv2/core/cuda.inl.hpp>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include "cglobal.h"

int usergraph_count=0;
int user_num=0;
Ptr<face::FaceRecognizer> model;
Wire* wire;
cv::dnn::Net vggface;
bool camera_run;
Q_DECLARE_METATYPE(cv::Mat)
int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>();
    QApplication a(argc, argv);
    Widget w;
    //w.setStyleSheet("background-color:black;");
    w.show();
    return a.exec();
}
