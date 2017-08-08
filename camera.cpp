#include "camera.h"
#include <QDebug>
#include <QMessageBox>

Camera::Camera(QByteArray deviceName)
{
    //統一解析度
    /*C310 Support
     * 320X240
     * 640X480
     * 1280 X 720
     * 5.0MP (軟體增強處理) */
    this->W = 640;
    this->H = 480;

    setCamera(deviceName);
}

void Camera::setCamera(QByteArray deviceName){
    if(deviceName == "")
        cameraDevice = new QCamera; //QCamera 控制相機用
    else
        cameraDevice = new QCamera(deviceName);

    connect(cameraDevice,SIGNAL(error(QCamera::Error)),this,SLOT(on_cameraError()));


    //==================================取景器 (Viewfinder)=============================
    /* 取景器就是，圖像抓進來的瞬間還沒完成編碼時，可以先從內部記憶體瀏覽看到圖。
     * 但不適合直接在取景器做圖像處理，頂多在上面畫畫。
     * 支援QVideoWidet、QGraphicsVidroItem、QAbstractVideoSurface，三種取景器。 */

    //3
    videoWidget = new VideoWidget(W,H);
    cameraDevice->setViewfinder(videoWidget->refVideoSurface());

    /*
    //1
    viewFinder = new QCameraViewfinder; //QCameraViewfinder 為取景器
    viewFinder->setFixedSize(480,320);
    ui->gridLayout->addWidget(viewFinder,0,1);
    camera->setViewfinder(viewFinder);
    */

    /*
    //2 QGraphicsVideoItem 的 camera.start() 一定要在事件啟動才有用 (bug?)
    videoItem = new QGraphicsVideoItem;
    camera->setViewfinder(videoItem);

    scene = new QGraphicsScene;
    scene->addItem(videoItem);
    ui->graphicsView->setScene(scene);
    //ui->graphicsView->scene()->addItem(videoItem); //簡寫
    */




    //================================捕捉模式========================================
    //有CaptureVireFinder(預覽)、CaptureStillImage(抓靜態圖片)、CaptureVideo(錄影)，三種捕捉模式

    //設定捕捉模式
    cameraDevice->setCaptureMode(QCamera::CaptureStillImage);




    //================================捕捉過程========================================
    //QCameraImageCapture 為設定捕捉過程，他本身不能捕捉

    //設定捕捉過程
    imageCapture = new QCameraImageCapture(cameraDevice);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer); //選擇將捕抓內容放在 檔案 或 記憶體

    //設定相機抓傳入時解析度
    QImageEncoderSettings imageSettings; //QImageEncoderSettings 為設定影像編碼選項
    //imageSettings.setCodec("image/jpeg");
    imageSettings.setResolution(W,H);
    imageCapture->setEncodingSettings(imageSettings); //匯入設定影像編碼選項

    //回傳相機支援
    /*
    qDebug() << imageCapture->supportedImageCodecs(); //回傳相機支援的影像編碼格式
    //回傳相繼支援的解析度
    QList<QSize> SupRes = imageCapture->supportedResolutions();
    for(int i =0;i<SupRes.size();i++)
        qDebug() << "width:" + QString::number(SupRes[i].width()) + " height:" + QString::number(SupRes[i].height());
    */



    //================================SINGAL========================================

    //error
    connect(imageCapture,SIGNAL(error(int,QCameraImageCapture::Error,QString)),
            this,SIGNAL(Error(int,QCameraImageCapture::Error,QString)));
    
    //4
    //SIGNAL imageCaptured 為已經捕捉，但還未儲存時
    //connect(imageCapture,SIGNAL(imageCaptured(int,QImage)),this,SIGNAL(imageCaptured(int,QImage)));

    //4
    //SIGNAL imageSaved 為已經儲存下來時
    //connect(imageCapture,SIGNAL(imageSaved(int,QString)),this,SIGNAL(imageSaved(int,QString)));
}

QImage Camera::getCurrentImage(){
    /* 這裡取圖時，一定要加 lock ，不然跑個幾小時就會當
       因為底層的程序在跑時，這邊程序又去取圖，有一定的機率會照成，底層正在改圖，這邊正在取圖
       這時候就會照成 segmentation fault */
    videoWidget->refImageSurface()->getlock();
    return videoWidget->refImageSurface()->getCurrentImage();
}

void Camera::drawVideoWidget(INFO info){
	/* 這裡也一樣，在畫圖時，一定要加 lock，不然跑幾個小時就會當
	   因為這邊的程序要改變座標變數時，paint 程序剛好使用此座標變數
	   這時候就會照成 segmentation fault (因為這原因 debug 了 10天..)*/
    videoWidget->lock();
    videoWidget->draw(info);
}

void Camera::CameraStrat(){
    cameraDevice->start();
}

void Camera::CameraStop(){
    if(cameraDevice->isAvailable())
        cameraDevice->stop();
}

void Camera::on_cameraError(){
    QMessageBox::critical(NULL,"Camera error",cameraDevice->errorString());
}


//================================ 4 ========================================

/*
void Camera::CameraUnlock(){
    cameraDevice->unlock();
}
*/

/*
void Camera::CameraCapture(){
    // searchAndLock() 為應用程序請求相機對焦，曝光或白平衡，完成後鎖定相機
    // (若相機沒有這些功能 或無法將完成訊號傳遞過來，這樣永遠都不會lock)
    //cameraDevice->searchAndLock();

    // 對於沒有對焦那些功能的相機，就直接在Unlock的情況下拍了
    // 但下面 imageCapture->capture 按快一點他就死當了
    if(imageCapture->isReadyForCapture()){
        imageCapture->capture("capture.jpg");
    }
    //所以對於不支援 QCameraImageCapture 的相機，就不用使用標記 4 的方式取圖。
}
*/

/*
void Camera::on_locked(){
    imageCapture->capture("capture.jpg"); //他不會blocking，函式結束時可能影像還未捕捉到，要從signal得知是否捕捉
}
*/

/*
void Camera::on_lockFailed(){
    //QMessageBox::critical(NULL,"Error","Lock Failed");
}
*/
