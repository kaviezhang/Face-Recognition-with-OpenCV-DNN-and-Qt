#ifndef FORM_REGISTION_H
#define FORM_REGISTION_H

#include <QWidget>

namespace Ui {
class Form_Registion;
}

class Form_Registion : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Registion(QWidget *main_win, QWidget *edit_win, QWidget *parent = 0);
    ~Form_Registion();
signals:

public slots:
    void back_to_edit();
    void back_complete_to_edit();
    void set_image(const QImage &img);
public:
    Ui::Form_Registion *ui;
};

#endif // FORM_REGISTION_H
