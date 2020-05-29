#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <QObject>
#include "faceDetector.h"
#include "camera.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QFileDialog>

#include <iostream>
#include <stdio.h>
#include "cglobal.h"
#include "wire.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QThread wire_thread;

signals:

public slots:
    void set_image(const QImage &img);
    void open_edit_slot();
    void open_result_slot(int);
    void exit();
    void main_sleep();
    void main_lock_sleep();

public:
    FaceDetector* faceDector_;
    Camera* camera_;
    QThread faceDetectThread_;
    QThread cameraThread_;
    QTimer *sleeptimer;

public:
    Ui::Widget *ui;
};

#endif // WIDGET_H
