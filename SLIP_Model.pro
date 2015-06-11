#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T20:40:02
#
#-------------------------------------------------

QT       += core gui\
         opengl\

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SLIP_Model
TEMPLATE = app


SOURCES += main.cpp\
    glwidget.cpp \
    simulation.cpp \
    hopper.cpp \
    mainwindow.cpp \
    chartsviewer.cpp \
    qcustomplot.cpp \
    control.cpp \
    backgroundsim.cpp \
    map.cpp

HEADERS  += \
    glwidget.h \
    simulation.h \
    hopper.h \
    mainwindow.h \
    chartsviewer.h \
    qcustomplot.h \
    control.h \
    backgroundsim.h \
    map.h

FORMS    += \
    mainwindow.ui \
    chartsviewer.ui

INCLUDEPATH += C:/boost_1_58_0
#LIBS += -L/usr/local/boost_1_48_0/stage/lib
