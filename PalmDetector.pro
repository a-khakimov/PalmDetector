TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt
QT += charts gui

SOURCES += \
        src/imageanalyser.cpp \
        src/main.cpp \
        src/window.cpp

RESOURCES += \
    qrc.qrc

INCLUDEPATH += \
    include

HEADERS += \
    include/imageanalyser.h \
    include/window.h
