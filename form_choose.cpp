#include "form_choose.h"
#include "ui_form_choose.h"
#include "form_edit.h"
#include "ui_form_edit.h"
#include "widget.h"
#include "ui_widget.h"
#include "form_registion.h"
#include "ui_form_registion.h"
#include <QTimer>
#include "cglobal.h"

QWidget *mainform;
Form_Choose::Form_Choose(QWidget *main_win,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Choose)
{
    ui->setupUi(this);
    setParent(parent);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    mainform = main_win;


    QString dir_str = "trainset/user"+QString::number(user_num);
    QDir dir;
    if(!dir.exists(dir_str))
    {
        //ui->editbutton->setText("Add");
        //ui->editbutton->setStyleSheet("border-image: url(:/new/src/img/addbutton.jpg);");
        ui->deletebutton->setVisible(false);
        //ui->usernumlabel->setText("User"+QString::number(user_num)+" is NULL");
        ui->usernumlabel->setPixmap(QPixmap::fromImage(QImage(":/new/src/img/usernull.jpg")));
    }else
    {
        QImage img;
        img.load(dir_str+"/0xxx.jpg");
        img=img.scaled(ui->usernumlabel->width(),ui->usernumlabel->height(),Qt::IgnoreAspectRatio);
        ui->usernumlabel->setPixmap(QPixmap::fromImage(img));
    }
    connect(ui->backbutton, SIGNAL(clicked(bool)), this, SLOT(back_to_edit()));
    connect(ui->editbutton, SIGNAL(clicked(bool)), this, SLOT(open_next_slot()));
    connect(ui->deletebutton, SIGNAL(clicked(bool)), this, SLOT(delete_userinfo()));

    //initial and start gpio buttons
    QTimer *timer=new QTimer(this);
    timer->setSingleShot(true);
    timer->start();
    connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_choose()));

    connect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_button_which_slot()));
    connect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
    connect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_edit()));

    load_table_chosen();
}

Form_Choose::~Form_Choose()
{
    delete ui;
}

void Form_Choose::open_next_slot()
{
    Form_Registion *form_registion=new Form_Registion((Widget*)mainform,(Form_Choose*)parentWidget(), this);
    form_registion->show();
    ui->backbutton->hide();
    ui->editbutton->hide();
    ui->deletebutton->hide();

    wire->run_=false;
    disconnect(wire,SIGNAL(signal_confirm(int)),this,SLOT(open_button_which_slot()));
    disconnect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
    disconnect(wire,SIGNAL(signal_exit(int)),this,SLOT(back_to_edit()));

}

void Form_Choose::back_to_edit()
{

    Form_Edit *form_edit=(Form_Edit*)parentWidget();
    form_edit->load_table_init();
    form_edit->load_table_img();
    form_edit->show();
    //form_edit->ui->openbutton->show();

    wire->run_=false;
    QTimer::singleShot(0,wire,SLOT(runSlot_edit()));

    connect(wire,SIGNAL(signal_confirm(int)),form_edit,SLOT(open_next_slot()));
    connect(wire,SIGNAL(signal_change(int)),form_edit,SLOT(load_table_chosen()));
    connect(wire,SIGNAL(signal_exit(int)),form_edit,SLOT(back_to_main()));


    form_edit->ui->backbutton->show();
    form_edit->ui->userinfo_widget->show();
    form_edit->ui->sleeplabel->show();
    delete(this);
}

void Form_Choose::back_to_edit_from_registion(QWidget *regis)
{

    Form_Edit *form_edit=(Form_Edit*)parentWidget();
    Form_Registion *form_regis=(Form_Registion*)regis;
    delete(form_regis);
    form_edit->load_table_init();
    form_edit->load_table_img();
    form_edit->show();
    //form_edit->ui->openbutton->show();

    wire->run_=false;
    QTimer::singleShot(0,wire,SLOT(runSlot_edit()));
    connect(wire,SIGNAL(signal_confirm(int)),form_edit,SLOT(open_next_slot()));
    connect(wire,SIGNAL(signal_change(int)),form_edit,SLOT(load_table_chosen()));
    connect(wire,SIGNAL(signal_exit(int)),form_edit,SLOT(back_to_main()));

    while(!((Widget*)mainform)->faceDector_->trained_label);
    form_edit->ui->backbutton->show();
    form_edit->ui->userinfo_widget->show();
    form_edit->ui->sleeplabel->show();
    delete(this);

    //((Widget*)mainform)->camera_->stopped();
    //((Widget*)mainform)->camera_->runSlot();
}

void Form_Choose::delete_userinfo()
{
    QString dir_str = "trainset/user"+QString::number(user_num);
    QDir dir;
    if(dir.exists(dir_str))
    {
        dir.setPath(dir_str);
        dir.removeRecursively();
    }
    this->ui->usernumlabel->clear();
    //Form_Edit *form_edit=(Form_Edit*)parent    //mainform->faceDector_->face_model_train();

    //wire->runSlot_edit();Widget();
    //form_edit->load_table_img();

    ((Widget*)mainform)->faceDector_->face_model_train();
}

void Form_Choose::load_table_chosen()
{
    button_choose++;
    if(button_choose>2){button_choose=1;}
    //loat the user info image
    //QPushButton *ui_item[2]={ui->editbutton,ui->deletebutton};
    QLabel *ui_item[2]={ui->editchosen,ui->deletechosen};

    for(int item=1;item<=2;item++)
    {
        if(button_choose==item)
        {
            //ui_item[item-1]->setStyleSheet("QPushButton{border:6px solid rgb(255,0,0)}");
            ui_item[item-1]->show();
        }
        if((button_choose-1==item)||(button_choose+1==item))
        {
            //ui_item[item-1]->setStyleSheet("QPushButton{border:2px solid rgb(0,0,0)}");
            ui_item[item-1]->hide();
        }
    }
}
void Form_Choose::open_button_which_slot()
{
    if(button_choose==1)
    {open_next_slot();}
    if(button_choose==2)
    {delete_userinfo();}
}
