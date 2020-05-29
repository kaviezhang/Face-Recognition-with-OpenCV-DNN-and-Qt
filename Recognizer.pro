#-------------------------------------------------
#
# Project created by KavieZhang@SJTU 2019-11-11T04:47:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Recognizer
TEMPLATE = app
DESTDIR = $$PWD
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += C:/Qt/minGW_build/install/include \
               C:\Qt\minGW_build\install\include\opencv2
LIBS += -L C:/Qt/minGW_build/lib/libopencv_*.a \

SOURCES += \
        main.cpp \
        widget.cpp \
        camera.cpp \
        faceDetector.cpp \
        form_edit.cpp \
        form_choose.cpp \
        form_registion.cpp \
        cglobal.cpp \
        wire.cpp \
    form_result.cpp \
    form_sleep.cpp

HEADERS += \
        widget.h \
        camera.h \
        faceDetector.h \
        form_edit.h \
        form_choose.h \
        form_registion.h \
        cglobal.h \
        wire.h \
    form_result.h \
    form_sleep.h

FORMS += \
        widget.ui \
        form_edit.ui \
        form_choose.ui \
        form_registion.ui \
    form_result.ui \
    form_sleep.ui

RESOURCES += \
    menu.qrc
