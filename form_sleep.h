#ifndef FORM_SLEEP_H
#define FORM_SLEEP_H

#include <QWidget>

namespace Ui {
class Form_Sleep;
}

class Form_Sleep : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Sleep(QString source, bool lock, QWidget *parent = 0);
    ~Form_Sleep();

public slots:
    void wakeup_edit();
    void wakeup_main();
    void wakeup_result();

    void lock_release();

public:
    Ui::Form_Sleep *ui;
};

#endif // FORM_SLEEP_H
