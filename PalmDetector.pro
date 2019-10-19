TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt
QT += charts gui

SOURCES += \
        imageanalyser.cpp \
        main.cpp \
        window.cpp

RESOURCES += \
    qrc.qrc

HEADERS += \
    imageanalyser.h \
    window.h
