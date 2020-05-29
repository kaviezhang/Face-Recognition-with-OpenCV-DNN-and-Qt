#ifndef FORM_CHOOSE_H
#define FORM_CHOOSE_H

#include <QWidget>

namespace Ui {
class Form_Choose;
}

class Form_Choose : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Choose(QWidget *main_win, QWidget *parent = 0);
    ~Form_Choose();
    int button_choose=0;
signals:

public slots:
    void open_next_slot();
    void open_button_which_slot();
    void back_to_edit();
    void back_to_edit_from_registion(QWidget*);
    void delete_userinfo();
    void load_table_chosen();

public:
    Ui::Form_Choose *ui;
};

#endif // FORM_CHOOSE_H
