#include "camera.h"
#include <QDebug>
#include <QMessageBox>

Camera::Camera()
{
    //統一解析度
    /*C310 Support
     * 320X240
     * 640X480
     * 1.2MP
     * 5.0MP    */
    this->W = 640;
    this->H = 480;
}

void Camera::setCamera(QByteArray deviceName){
    if(deviceName == "")
        cameraDevice = new QCamera; //QCamera 控制相機用
    else
        cameraDevice = new QCamera(deviceName);

    connect(cameraDevice,SIGNAL(error(QCamera::Error)),this,SLOT(on_cameraError()));

    //viewfinder
    videoWidget = new VideoWidget(W,H);
    cameraDevice->setViewfinder(videoWidget->refVideoSurface());

    //設定捕捉模式
    cameraDevice->setCaptureMode(QCamera::CaptureStillImage);

    //設定捕捉過程
    imageCapture = new QCameraImageCapture(cameraDevice); //QCameraImageCapture 為設定捕捉過程，他本身不能捕捉
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer); //選擇將捕抓內容放在 檔案 或 記憶體

    //設定相機抓傳入時解析度
    QImageEncoderSettings imageSettings; //QImageEncoderSettings 為設定影像編碼選項
    //imageSettings.setCodec("image/jpeg");
    imageSettings.setResolution(W,H);
    imageCapture->setEncodingSettings(imageSettings); //匯入設定影像編碼選項

    //error
    connect(imageCapture,SIGNAL(error(int,QCameraImageCapture::Error,QString)),
            this,SIGNAL(Error(int,QCameraImageCapture::Error,QString)));
}

QImage Camera::getCurrentImage(){
    videoWidget->refImageSurface()->getlock();
    return videoWidget->refImageSurface()->getCurrentImage();
}

void Camera::drawVideoWidget(QRect *rects , int rectCount){
    videoWidget->draw(rects,rectCount);
}

void Camera::CameraStrat(){
    cameraDevice->start();
}

void Camera::CameraStop(){
    cameraDevice->stop();
}

void Camera::on_cameraError(){
    QMessageBox::critical(NULL,"Camera error",cameraDevice->errorString());
}
