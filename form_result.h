#ifndef FORM_RESULT_H
#define FORM_RESULT_H

#include <QWidget>
#include <camera.h>
namespace Ui {
class Form_Result;
}

class Form_Result : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Result(int user_num,Camera *camera_,QWidget *parent = 0);
    ~Form_Result();
    Camera *camera;

public slots:
    void back_to_main();
    void set_led_status();
    void result_sleep();

public:
    Ui::Form_Result *ui;

};

#endif // FORM_RESULT_H
