#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QWidget>
#include <QCamera>
#include <QCameraImageCapture>
#include <QImageEncoderSettings>

#include "videowidget.h"
#include "myvideosurface.h" //ImageFrame

class Camera : public QObject
{
    Q_OBJECT
public:
    Camera();
    //set
    void setCamera(QByteArray deviceName);
    void CameraStrat();
    void CameraStop();

    //get
    QWidget *getVideoWidget() { return videoWidget; } //丟出畫版
    ImageFrame getCurrentImage() { return videoWidget->refImageSurface()->getCurrentImage(); } //丟圖

private:
    //camera and control
    QCamera *cameraDevice;
    QCameraImageCapture *imageCapture;

    //viewfinder
    VideoWidget *videoWidget;

    //屬性
    int W;
    int H;

private slots:
    void on_cameraError();

signals:
    void Error(int id,QCameraImageCapture::Error error,const QString &errorString);
};

#endif // CAMERA_H
