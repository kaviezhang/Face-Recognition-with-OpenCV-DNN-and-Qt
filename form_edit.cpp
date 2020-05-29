#include "form_edit.h"
#include "ui_form_edit.h"
#include "widget.h"
#include "ui_widget.h"
#include "form_choose.h"
#include "ui_form_choose.h"
#include "form_sleep.h"
#include "ui_form_sleep.h"
#include <QMouseEvent>
#include <QTimer>
#include "cglobal.h"


Form_Edit::Form_Edit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Edit)
{
    ui->setupUi(this);
    setParent(parent);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    load_table_init();
    load_table_img();

    connect(ui->backbutton, SIGNAL(clicked(bool)), this, SLOT(back_to_main()));
    //connect(ui->openbutton, SIGNAL(clicked(bool)), this, SLOT(open_next_slot()));
    //Widget *mainform=(Widget*)parentWidget();
    //connect(mainform->faceDector_, SIGNAL(image_signal(QImage)), this, SLOT(set_image(QImage)));

    ui->user1label->installEventFilter(this);
    ui->user2label->installEventFilter(this);
    ui->user3label->installEventFilter(this);
    ui->user4label->installEventFilter(this);
    ui->user5label->installEventFilter(this);
    ui->user6label->installEventFilter(this);
    ui->sleeplabel->installEventFilter(this);

    //open_next_slot_event(user_num);
    QTimer *timer=new QTimer(this);
    timer->setSingleShot(true);
    timer->start();
    connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_edit()));

    connect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_next_slot()));
    connect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
    connect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_main()));
}

Form_Edit::~Form_Edit()
{
    delete ui;
}

void Form_Edit::back_to_main()
{
    Widget *mainform=(Widget*)parentWidget();

    wire->run_=false;
    QTimer::singleShot(0,wire,SLOT(runSlot_widget()));

    connect(wire,SIGNAL(signal_confirm(int)),mainform,SLOT(open_edit_slot()));
    //disconnect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
    user_num=0;

    connect(mainform->faceDector_,SIGNAL(result_signal(int)),mainform,SLOT(open_result_slot(int)));
    camera_run=true;

    mainform->show();
    mainform->ui->camerashowlable->show();
    mainform->ui->editbutton->show();
    mainform->ui->exitbutton->show();
    delete(this);
}
void Form_Edit::set_image(const QImage &img)
{
 ui->user1label->setPixmap(QPixmap::fromImage(img));
}
void Form_Edit::open_next_slot()
{
    if(user_num<7)
    {
    Form_Choose *form_choose=new Form_Choose((Widget*)parentWidget(), this);
    //form_choose->ui->usernumlabel->setText("user is NO."+QString::number(user_num));
    form_choose->show();
    ui->userinfo_widget->hide();
    ui->backbutton->hide();
    ui->sleeplabel->hide();

    wire->run_=false;
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_next_slot()));
    disconnect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
    disconnect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_main()));
    }
    else
    {
    edit_sleep();
    }
}
void Form_Edit::open_next_slot_event(int num)
{
    user_num=num;
    Form_Choose *form_choose=new Form_Choose((Widget*)parentWidget(), this);
    //form_choose->ui->usernumlabel->setText("user is NO."+QString::number(user_num));
    form_choose->show();
    ui->userinfo_widget->hide();
    ui->backbutton->hide();
    ui->sleeplabel->hide();

    wire->run_=false;
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_next_slot()));
    disconnect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
}
bool Form_Edit::eventFilter(QObject *watched, QEvent *event)
{
    if((qobject_cast<QLabel*>(watched)==ui->user1label)&&(event->type()==QEvent::MouseButtonRelease))
    {
        open_next_slot_event(1);
        return true;
    }
    else if((qobject_cast<QLabel*>(watched)==ui->user2label)&&(event->type()==QEvent::MouseButtonRelease))
    {
        open_next_slot_event(2);
        return true;
    }
    else if((qobject_cast<QLabel*>(watched)==ui->user3label)&&(event->type()==QEvent::MouseButtonRelease))
    {
        open_next_slot_event(3);
        return true;
    }
    else if((qobject_cast<QLabel*>(watched)==ui->user4label)&&(event->type()==QEvent::MouseButtonRelease))
    {
        open_next_slot_event(4);
        return true;
    }
    else if((qobject_cast<QLabel*>(watched)==ui->user5label)&&(event->type()==QEvent::MouseButtonRelease))
    {
        open_next_slot_event(5);
        return true;
    }
    else if((qobject_cast<QLabel*>(watched)==ui->user6label)&&(event->type()==QEvent::MouseButtonRelease))
    {
        open_next_slot_event(6);
        return true;
    }
    else if((qobject_cast<QLabel*>(watched)==ui->sleeplabel)&&(event->type()==QEvent::MouseButtonRelease))
    {
        edit_sleep();
        return true;
    }
    else
    {
        return false;
    }
}
void Form_Edit::load_table_img()
{
    //loat the user info image
    QLabel *ui_item[6]={ui->user1label,ui->user2label,ui->user3label,ui->user4label,ui->user5label,ui->user6label};
    for(int item=1;item<=6;item++)
    {
    QImage img_show;
    QString dir_str = "trainset/user"+QString::number(item);
    if(img_show.load(dir_str+"/0xxx.jpg")){
    img_show=img_show.scaled(ui->user1label->width(),ui->user1label->height(),Qt::IgnoreAspectRatio);
    ui_item[item-1]->setPixmap(QPixmap::fromImage(img_show));}
    }
}

void Form_Edit::load_table_chosen()
{
    user_num++;
    if(user_num>7){user_num=1;}
    //loat the user info image
    QLabel *ui_item[7]={ui->user1label,ui->user2label,ui->user3label,ui->user4label,ui->user5label,ui->user6label,ui->sleeplabel};
    for(int item=1;item<=7;item++)
    {
        if(user_num==item)
        {ui_item[item-1]->setStyleSheet("QLabel{border:6px solid rgb(255,0,0)}");}
        if((user_num-1==item)||(user_num+6==item))
        {//ui_item[item-1]->setStyleSheet("QLabel{border:none}");
         ui_item[item-1]->setStyleSheet("background-color:rgb(255,255,255)");
        }
    }
}
void Form_Edit::load_table_init()
{
    user_num=1;
    //loat the user info image
    QLabel *ui_item[7]={ui->user1label,ui->user2label,ui->user3label,ui->user4label,ui->user5label,ui->user6label,ui->sleeplabel};
    for(int item=1;item<=7;item++)
    {
        if(user_num==item)
            {ui_item[item-1]->setStyleSheet("QLabel{border:6px solid rgb(255,0,0)}");}
        else{
            ui_item[item-1]->setStyleSheet("background-color:rgb(255,255,255)");
            }
    }
    ui->sleeplabel->setStyleSheet("QLabel{border:1px solid rgb(0,0,0)}");
}

void Form_Edit::edit_sleep()
{
    Form_Sleep *form_sleep=new Form_Sleep("edit",false,this);
    //form_choose->ui->usernumlabel->setText("user is NO."+QString::number(user_num));
    form_sleep->show();
    ui->userinfo_widget->hide();
    ui->backbutton->hide();
    ui->sleeplabel->hide();

    wire->run_=false;
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_next_slot()));
    disconnect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
    disconnect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_main()));
}
