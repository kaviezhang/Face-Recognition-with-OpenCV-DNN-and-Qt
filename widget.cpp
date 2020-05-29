#include "widget.h"
#include "ui_widget.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <linux/videodev.h>
#include <QTimer>
#include "form_edit.h"
#include "ui_form_edit.h"
#include "form_result.h"
#include "ui_form_result.h"
#include "form_sleep.h"
#include "ui_form_sleep.h"
#include "cglobal.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    setParent(parent);
//    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    camera_ = new Camera();
    faceDector_ = new FaceDetector();

    faceDetectThread_.start();
    cameraThread_.start();

    camera_->moveToThread(&cameraThread_);
    faceDector_->moveToThread(&faceDetectThread_);

    //gpio button thread defines
    wire=new Wire();
    wire_thread.start();
    wire->moveToThread(&wire_thread);

    // TODO: Add in slot to turn off camera_, or something
    connect(faceDector_, SIGNAL(image_signal(QImage)), this, SLOT(set_image(QImage)));

    connect(camera_, SIGNAL(matReady(cv::Mat)), faceDector_, SLOT(processFrame(cv::Mat)));
    QTimer *timer=new QTimer(this);
    timer->setSingleShot(true);
    timer->start();
    connect(timer, SIGNAL(timeout()), camera_, SLOT(runSlot()));
    connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_widget()));

    connect(ui->editbutton, SIGNAL(clicked(bool)), this, SLOT(open_edit_slot()));
    connect(ui->exitbutton,SIGNAL(clicked(bool)),this,SLOT(exit()));

    connect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_edit_slot()));

    connect(faceDector_,SIGNAL(result_signal(int)),this,SLOT(open_result_slot(int)));

    connect(faceDector_,SIGNAL(lock_signal()),this,SLOT(main_lock_sleep()));
}

Widget::~Widget()
{
    delete ui;
}
void Widget::set_image(const QImage &img)
{
 ui->camerashowlable->setPixmap(QPixmap::fromImage(img));
}
void Widget::open_edit_slot()
{
    Form_Edit *form_edit=new Form_Edit(this);
    form_edit->show();
    ui->camerashowlable->hide();
    ui->exitbutton->hide();
    ui->editbutton->hide();
    wire->run_=false;
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_edit_slot()));
    disconnect(faceDector_,SIGNAL(result_signal(int)),this,SLOT(open_result_slot(int)));
    camera_run=false;
}
void Widget::exit()
{
    QApplication *app;
    app->quit();
    //this->close();
}

void Widget::open_result_slot(int label)
{
    camera_run=false;
    wire->run_=false;
    Form_Result *form_result=new Form_Result(label,camera_,this);
    form_result->show();
    ui->camerashowlable->hide();
    ui->exitbutton->hide();
    ui->editbutton->hide();
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_edit_slot()));
}

void Widget::main_sleep()
{
    Form_Sleep *form_sleep=new Form_Sleep("main",false,this);
    form_sleep->show();
    ui->camerashowlable->hide();
    ui->exitbutton->hide();
    ui->editbutton->hide();
    wire->run_=false;
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_edit_slot()));
    disconnect(faceDector_,SIGNAL(result_signal(int)),this,SLOT(open_result_slot(int)));
    camera_run=false;
}

void Widget::main_lock_sleep()
{
    Form_Sleep *form_sleep=new Form_Sleep("main",true,this);
    form_sleep->show();
    ui->camerashowlable->hide();
    ui->exitbutton->hide();
    ui->editbutton->hide();
    wire->run_=false;
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_edit_slot()));
    disconnect(faceDector_,SIGNAL(result_signal(int)),this,SLOT(open_result_slot(int)));
    camera_run=false;
}
