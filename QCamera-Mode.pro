#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T14:03:20
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCamera-Mode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myvideosurface.cpp \
    videowidget.cpp \
    camera.cpp \
    scanthread.cpp \
    shared.cpp

HEADERS  += mainwindow.h \
    myvideosurface.h \
    videowidget.h \
    camera.h \
    scanthread.h \
    shared.h

FORMS    += mainwindow.ui

macx{
    #libdmtx
    INCLUDEPATH += /usr/local/include
    LIBS += `pkg-config --libs libdmtx`
}
unix:!macx{
    #libdmtx
    INCLUDEPATH += /home/levy/raspi/sysroot/usr/include
    LIBS += -L/home/levy/raspi/sysroot/usrlib -ldmtx

    #Cross Compile
    target.path = /home/pi/test5
    INSTALLS += target
}
win32{
    #preprocessor definition
    DEFINES += WINDOWS

    #libdmtx
    INCLUDEPATH += C:\libdmtx-libdmtx\include
    LIBS += C:\libdmtx-libdmtx\libdmtx.lib
}
