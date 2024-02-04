QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# CONFIG += link_pkgconfig
# PKGCONFIG += python

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += no_keywords
SOURCES += \
    facedetectionthread.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    facedetectionthread.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += C:\Users\my\Downloads\release\release\install\include\
LIBS += -LC:\Users\my\Downloads\release\release\bin -lopencv_calib3d490 -lopencv_video490 -lopencv_videoio490

DEPENDPATH +=  C:\opencv\release\install\include





