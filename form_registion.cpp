#include "form_registion.h"
#include "ui_form_registion.h"
#include "form_choose.h"
#include "ui_form_choose.h"
#include "form_edit.h"
#include "ui_form_edit.h"
#include "widget.h"
#include "ui_widget.h"

#include "cglobal.h"
Form_Edit *form_edit;
Widget *main_form;
Form_Registion::Form_Registion(QWidget *main_win, QWidget *edit_win, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Registion)
{
    ui->setupUi(this);
    setParent(parent);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    main_form=(Widget*)main_win;
    usergraph_count=0;
    main_form->faceDector_->setProcessMode(false);
    connect(main_form->faceDector_, SIGNAL(image_signal(QImage)), this, SLOT(set_image(QImage)));
    form_edit=(Form_Edit*)edit_win;
    connect(ui->backbutton, SIGNAL(clicked(bool)), this, SLOT(back_to_edit()));

    camera_run=true;
}

Form_Registion::~Form_Registion()
{
    delete ui;
}

void Form_Registion::set_image(const QImage &img)
{
 ui->camerashowlable->setPixmap(QPixmap::fromImage(img));
 if(usergraph_count>=2)
 {

     usergraph_count=0;
     main_form->faceDector_->trained_label=false;
     //main_form->cameraThread_.msleep(1);
     main_form->faceDector_->face_model_train();
     //while(!main_form->faceDector_->trained_label);
     main_form->faceDector_->setProcessMode(true);
     back_complete_to_edit();
 }
}

void Form_Registion::back_to_edit()
{
    Form_Choose *form_choose=(Form_Choose*)parentWidget();
    //delete the uncompleted user graph path
    QString dir_str = "trainset/user"+QString::number(user_num);
    QDir dir;
    if(dir.exists(dir_str))
    {
        dir.setPath(dir_str);
        dir.removeRecursively();
    }
    //user_num=1;
    form_choose->back_to_edit_from_registion(this);
    main_form->faceDector_->setProcessMode(true);
}
void Form_Registion::back_complete_to_edit()
{
    Form_Choose *form_choose=(Form_Choose*)parentWidget();
    //user_num=1;
    form_choose->back_to_edit_from_registion(this);
    main_form->faceDector_->setProcessMode(true);
}
