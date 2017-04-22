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
    TmpID = -1;

    //scanner
    scanner = new scanthread(this);
    connect(camera,SIGNAL(Error(int,QCameraImageCapture::Error,QString)),
            this,SLOT(displayCaptureError(int,QCameraImageCapture::Error,QString)));

    //initialization
    initialization();

    //draw
    connect(scanner,SIGNAL(throwInfo(QRect*,int)),camera,SLOT(drawVideoWidget(QRect*,int))); //一定要在 initialization() 之後

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

ImageFrame MainWindow::on_Capture(){
    //ImageFrame *currentImage_ptr = camera->getCurrentImage();

    //by value (當一個傳送的中繼站，怕後面處理太久，前面圖系統會突然自動release)
    //this->currentImage.id = currentImage_ptr->id;
    //this->currentImage.image = currentImage_ptr->image; //這裡不用加 copy() 也是 by value

    //by value
    this->currentImage =  camera->getCurrentImage();

    //若圖有重複則記處
    if(currentImage.id != TmpID)
        this->currentImage.isRepeat = false;
    else
        this->currentImage.isRepeat = false;

    TmpID = currentImage.id; //update

    return this->currentImage;
}

void MainWindow::displayCaptureError(int id, QCameraImageCapture::Error error, const QString &errorString){
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
}

MainWindow::~MainWindow()
{
    stop();
    delete ui;
}
