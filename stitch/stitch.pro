QT += quick

CONFIG += c++11

INCLUDEPATH += /home/ccs/opencv/build-V700/install/include/opencv4
LIBS += -L/home/ccs/opencv/build-V700/install/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_calib3d -lopencv_imgproc -lopencv_flann -lopencv_features2d -lopencv_dnn -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_xfeatures2d


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    main.h
