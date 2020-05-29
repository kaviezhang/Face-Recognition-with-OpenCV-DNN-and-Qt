#ifndef FORM_EDIT_H
#define FORM_EDIT_H

#include <QWidget>

namespace Ui {
class Form_Edit;
}

class Form_Edit : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Edit(QWidget *parent = 0);
    ~Form_Edit();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
signals:

public slots:
    void set_image(const QImage &img);
    void open_next_slot();
    void open_next_slot_event(int);
    void back_to_main();
    void load_table_chosen();
    void load_table_init();
    void edit_sleep();

public:
    Ui::Form_Edit *ui;
    void load_table_img();

};

#endif // FORM_EDIT_H
