#include "form_result.h"
#include "ui_form_result.h"
#include "widget.h"
#include "ui_widget.h"
#include "form_sleep.h"
#include "ui_form_sleep.h"
#include <QMouseEvent>
#include <QTimer>
#include <string>
#include "cglobal.h"

Form_Result::Form_Result(int label,Camera *camera_,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Result)
{
    ui->setupUi(this);
    setParent(parent);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    //if(label==0)ui->label->setText("Welcome to User_unknown!");
    //else{ui->label->setText("Welcome to User"+QString::number(label)+"!");}
    connect(ui->backbutton, SIGNAL(clicked(bool)), this, SLOT(back_to_main()));
    connect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_main()));
    camera=camera_;

    QTimer *timer=new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_result()));
    timer->start();

//    QTimer *timer_=new QTimer(this);
//    timer_->setSingleShot(true);
//    connect(timer_, SIGNAL(timeout()), this, SLOT(set_led_status()));
//    timer->start(500);

    QTimer::singleShot(2000,this,SLOT(set_led_status()));

    QTimer::singleShot(20000,this,SLOT(result_sleep()));

}

Form_Result::~Form_Result()
{
    delete ui;
}

void Form_Result::back_to_main()
{
    Widget *mainform=(Widget*)parentWidget();
    //mainform->faceDector_->face_model_train();
    disconnect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_main()));
    connect(wire,SIGNAL(signal_confirm(int)),mainform,SLOT(open_edit_slot()));

    wire->run_=false;
    QTimer::singleShot(0,wire,SLOT(runSlot_widget()));

    camera_run=true;

    ui->label->hide();
    ui->backbutton->hide();
    mainform->show();
    mainform->ui->camerashowlable->show();
    mainform->ui->editbutton->show();
    mainform->ui->exitbutton->show();
    delete(this);
}

void Form_Result::set_led_status()
{
    wire->led_status=false;
}
void Form_Result::result_sleep()
{
    Form_Sleep *form_sleep=new Form_Sleep("result",false,this);
    form_sleep->show();
    ui->backbutton->hide();
    ui->label->hide();
    wire->run_=false;
    disconnect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_main()));
}
