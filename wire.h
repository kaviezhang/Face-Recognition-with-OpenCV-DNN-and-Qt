#ifndef WIRE_H
#define WIRE_H

#include <QObject>
#include <QScopedPointer>
#include <QTimerEvent>
#include <stdio.h>
#include <iostream>
#include <QImage>
#include <QBasicTimer>
#include <QDebug>
class Wire : public QObject
{
    Q_OBJECT
    int status;

public:
    Wire();
    bool run_=true;
    bool led_status=true;

signals:
    void signal_change(int);
    void signal_confirm(int);
    void signal_exit(int);
    void signal_wakeup(int);
public slots:
    void runSlot_widget();
    void runSlot_edit();
    void runSlot_choose();
    void runSlot_result();
};

#endif // WIRE_H
