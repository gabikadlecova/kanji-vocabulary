#-------------------------------------------------
#
# Project created by QtCreator 2018-12-10T16:27:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kanjiapp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        kanjiwidget.cpp \
    editwidget.cpp \
    detailswidget.cpp \
    trainwidget.cpp \
    kanjilistwidget.cpp \
    mainwidget.cpp \
    kanjiapp/KanjiData.cpp \
    detailswidget.cpp \
    editwidget.cpp \
    kanjilistwidget.cpp \
    kanjiwidget.cpp \
    main.cpp \
    mainwidget.cpp \
    trainwidget.cpp \
    addkanjiwidget.cpp \
    filterdialog.cpp

HEADERS += \
        kanjiwidget.h \
    editwidget.h \
    detailswidget.h \
    trainwidget.h \
    kanjilistwidget.h \
    mainwidget.h \
    kanjiapp/KanjiData.h \
    detailswidget.h \
    editwidget.h \
    kanjilistwidget.h \
    kanjiwidget.h \
    mainwidget.h \
    trainwidget.h \
    addkanjiwidget.h \
    filterdialog.h

FORMS += \
        kanjiwidget.ui \
    editwidget.ui \
    detailswidget.ui \
    trainwidget.ui \
    kanjilistwidget.ui \
    mainwidget.ui \
    addkanjiwidget.ui \
    filterdialog.ui

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
