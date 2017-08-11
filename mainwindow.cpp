#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QCameraInfo>
#include <QSpacerItem>

#include "scanthread.h"
scanthread *scanner;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //為了讓camera在中心
    QSpacerItem * H_spacer = new QSpacerItem(1000,0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem * H_spacer_2 = new QSpacerItem(1000,0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->gridLayout->addItem(H_spacer,0,0);
    ui->gridLayout->addItem(H_spacer_2,0,2);

    //camera
    camera = new Camera;

    //scanner
    scanner = new scanthread(this);
    connect(camera,SIGNAL(Error(int,QCameraImageCapture::Error,QString)),
            this,SLOT(displayCaptureError(int,QCameraImageCapture::Error,QString)));

    //initialization
    initialization();

    //draw(一定要在 initialization() 之後)
    qRegisterMetaType<INFO>("INFO");
    /* 下面設定 BlockingQueuedConnection 是重點！ (他會 blocking emit 的地方，直到 slot 處理完)
       一定要等 paint 值送到在繼續處理，不然跑幾小時就當了，因為這原因 debug 好久..*/
    connect(scanner,SIGNAL(throwInfo(INFO)),camera,SLOT(drawVideoWidget(INFO)),Qt::BlockingQueuedConnection);

    //ui
    formText = new QPlainTextEdit(this);
    ui->gridLayout->addWidget(formText,1,0,1,3);
    ui->gridLayout->addWidget(camera->getVideoWidget(),0,1); //一定要在 initialization() 之後
}

bool MainWindow::initialization(){
    //列出相機
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    qDebug() << cameras[0].deviceName(); //設備id
    qDebug() << cameras[0].description(); //設備名稱

    //camera set
    camera->setCamera(cameras[0].deviceName().toLocal8Bit());
    camera->CameraStrat();

    /* 4
    connect(camera[i],SIGNAL(imageCaptured(int,QImage)),this,SLOT(on_imageCaptured(int,QImage)));
    connect(camera[i],SIGNAL(imageSaved(int,QString)),this,SLOT(on_imageSaved(int,QString)));
    */

    scanner->start();

    return true;
}

void MainWindow::start(){
    if(scanner->isRunning())
        return;

    initialization();
}

void MainWindow::stop(){
    camera->CameraStop();
    scanner->stop();
    qDebug() << "stop";
}

QImage &MainWindow::on_Capture(){
    return camera->getCurrentImage(); //by MyVideoSurface 的 currentImage reference
}

void MainWindow::displayCaptureError(int id, QCameraImageCapture::Error error, const QString &errorString){
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
}

/*//2
void MainWindow::keyPressEvent(QKeyEvent *event){
    Q_UNUSED(event);
    
    //如果使用 QGraphicsVideoItem，若沒有在事件啟動 camera->start();，會沒有反應
    camera->start();
}
*/

/*//4
void MainWindow::on_imageCaptured(int id, const QImage &preview){
    qDebug() << "on_imageCaptured: " + QString::number(id);
   la_captured[0]->setPixmap(QPixmap::fromImage(preview));
   camera->CameraUnlock(); //捕捉後即可解鎖，因為已經進來了，所以不用等saved才解鎖
}
*/

/*//4
void MainWindow::on_imageSaved(int id, const QString &fileName){
    qDebug() << "on_imageSaved: " + QString::number(id);
    scene->clear();
    scene->addPixmap(QPixmap(fileName)); //從檔案中call
}
*/

MainWindow::~MainWindow()
{
    stop();
    delete ui;
}
