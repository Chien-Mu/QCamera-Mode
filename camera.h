#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QWidget>
#include <QCamera>
#include <QCameraImageCapture>
#include <QImageEncoderSettings>

#include "videowidget.h"
#include "shared.h"

#include <QCameraViewfinder> //1
#include <QGraphicsVideoItem> //2

/* QCamera 官網範例：https://doc.qt.io/qt-5/qtmultimedia-multimediawidgets-camera-camera-cpp.html*/
// 標記 // 1 viewfinder 使用 QCameraViewfinder
// 標記 // 2 viewfinder 使用 QGraphicsVideoItem
// 標記 // 3 viewfinder 使用 QAbstractVideoSurface (主要使用)
// 標記 // 4 是為了等待相機對焦好才拍照的項目

class Camera : public QObject
{
    Q_OBJECT
public:
    Camera();
    //set
    void setCamera(QByteArray deviceName);
    void CameraStrat();
    void CameraStop();
    //void CameraUnlock(); //4
    //void CameraCapture(); //4

    //get
    QWidget *getVideoWidget() { return videoWidget; } //丟出畫版
    QImage &getCurrentImage(); //丟圖(不使用 標記 4 的情況，就是使用這函式取圖)

public slots:
    void drawVideoWidget(INFO info);

private:
    //camera and control
    QCamera *cameraDevice;
    QCameraImageCapture *imageCapture;

    //viewfinder
    //QCameraViewfinder *viewFinder; //1
    //QGraphicsVideoItem *videoItem; //2
    VideoWidget *videoWidget; //3

    //2
    QGraphicsScene *scene;

    //屬性
    QSize imageSize;

private slots:
    void on_cameraError();
    //void on_locked(); //4
    //void on_lockFailed(); //4

signals:
    void Error(int id,QCameraImageCapture::Error error,const QString &errorString);
    //void imageCaptured(int id,const QImage &preview); //4
    //void imageSaved(int id,const QString &fileName); //4
};

#endif // CAMERA_H
