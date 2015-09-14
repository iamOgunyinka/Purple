#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T10:45:11
#
#-------------------------------------------------

QT       += core gui network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Purple
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++11 -Werror

SOURCES += main.cpp\
    mainwindow.cpp \
    videostructure.cpp \
    videotablemodel.cpp \
    searchdialog.cpp \
    extractor.cpp \
    downloadmanager.cpp \
    videodetails.cpp \
    synchronizednetworkaccessmanager.cpp

HEADERS  += \
    mainwindow.hpp \
    videostructure.hpp \
    videotablemodel.hpp \
    searchdialog.hpp \
    extractor.hpp \
    downloadmanager.hpp \
    videodetails.hpp \
    filterfunctions.hpp \
    synchronizednetworkaccessmanager.hpp

FORMS    +=

RESOURCES += \
    images/images.qrc
