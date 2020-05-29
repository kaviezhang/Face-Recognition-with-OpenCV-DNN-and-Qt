#include "form_sleep.h"
#include "ui_form_sleep.h"
#include "form_edit.h"
#include "ui_form_edit.h"
#include "widget.h"
#include "ui_widget.h"
#include "form_result.h"
#include "ui_form_result.h"
#include <QTimer>
#include "wire.h"
#include "cglobal.h"

Form_Sleep::Form_Sleep(QString source, bool lock, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Sleep)
{
    ui->setupUi(this);
    setParent(parent);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    if(source=="edit")
    {
    connect(wire,SIGNAL(signal_wakeup(int)),this,SLOT(wakeup_edit()));

    QTimer *timer=new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_result()));
    timer->start();
    }
    else if(source=="main")
    {
        connect(wire,SIGNAL(signal_wakeup(int)),this,SLOT(wakeup_main()));
        if(lock)
        {
            QTimer::singleShot(60000,this,SLOT(lock_release()));
        }
        else
        {   QTimer *timer=new QTimer(this);
            timer->setSingleShot(true);
            connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_result()));
            timer->start();
        }
    }
    else//"result"
    {
        connect(wire,SIGNAL(signal_wakeup(int)),this,SLOT(wakeup_result()));
        QTimer *timer=new QTimer(this);
        timer->setSingleShot(true);
        connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_result()));
        timer->start();
    }
}
void Form_Sleep::lock_release()
{
    QTimer *timer=new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), wire, SLOT(runSlot_result()));
    timer->start();
}
Form_Sleep::~Form_Sleep()
{
    delete ui;
}

void Form_Sleep::wakeup_edit()
{
    Form_Edit *form_edit=(Form_Edit*)parentWidget();
    form_edit->load_table_init();
    form_edit->load_table_img();
    form_edit->show();
    disconnect(wire,SIGNAL(signal_wakeup(int)),this,SLOT(wakeup_edit()));

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

void Form_Sleep::wakeup_main()
{
    Widget *mainform=(Widget*)parentWidget();
    mainform->show();

    wire->run_=false;
    QTimer::singleShot(0,wire,SLOT(runSlot_widget()));

    connect(wire,SIGNAL(signal_confirm(int)),mainform,SLOT(open_edit_slot()));
    //disconnect(wire,SIGNAL(signal_change(int)),this,SLOT(load_table_chosen()));
    user_num=0;

    connect(mainform->faceDector_,SIGNAL(result_signal(int)),mainform,SLOT(open_result_slot(int)));
    camera_run=true;

    mainform->ui->camerashowlable->show();
    mainform->ui->editbutton->show();
    mainform->ui->exitbutton->show();
    delete(this);
}
void Form_Sleep::wakeup_result()
{
    Form_Result *form_result=(Form_Result*)parentWidget();
    form_result->show();

    wire->run_=false;
    QTimer::singleShot(0,wire,SLOT(runSlot_result()));

    connect(wire,SIGNAL(signal_exit(int)),form_result,SLOT(back_to_main()));
    form_result->ui->backbutton->show();
    form_result->ui->label->show();
    delete(this);
}
