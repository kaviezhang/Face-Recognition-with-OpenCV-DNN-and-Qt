#ifndef CGLOBAL_H
#define CGLOBAL_H
#include <opencv2/face.hpp>
#include <opencv2/core.hpp>
#include "wire.h"
#include <opencv2/dnn/dnn.hpp>

extern int usergraph_count;
extern int user_num;
using namespace cv;
extern Ptr<face::FaceRecognizer> model;
extern Wire* wire;
extern cv::dnn::Net vggface;
extern bool camera_run;
class CGlobal
{
public:
    CGlobal();
    ~CGlobal();
};

#endif // CGLOBAL_H
