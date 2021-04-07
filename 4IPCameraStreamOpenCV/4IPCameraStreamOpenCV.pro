#-------------------------------------------------
#
# Project created by QtCreator 2017-02-26T14:13:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += /home/ccs/opencv/build-V700/install/include/opencv4
LIBS += -L/home/ccs/opencv/build-V700/install/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_calib3d -lopencv_imgproc -lopencv_flann -lopencv_features2d -lopencv_dnn -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio
#-lopencv_videostab -lopencv_shape  -lopencv_superres

#TARGET = opencv_cpp_show_webcam_stream_on_Qt_GUI
#TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

