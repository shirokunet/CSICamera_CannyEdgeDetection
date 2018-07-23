#-------------------------------------------------
#
# Project created by QtCreator 2018-07-21T01:52:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_csicamera
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG += c++11
INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_videostab

LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_highgui
LIBS += -lopencv_video
LIBS += -lopencv_videoio
LIBS += -lopencv_videostab
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc

LIBS += -lopencv_cudaarithm
LIBS += -lopencv_cudacodec
LIBS += -lopencv_cudafilters
LIBS += -lopencv_cudaimgproc
